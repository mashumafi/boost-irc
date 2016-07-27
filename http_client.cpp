#include "http_client.hpp"

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>

using boost::asio::ip::tcp;

http_client* http_client::create(const std::string& server, const std::string& path, const boost::function<void (const std::stringstream&)>& json_cb)
{
  return new http_client(server, path, json_cb);
}

http_client::~http_client()
{
  io_service.stop();
}

http_client::http_client(const std::string& server, const std::string& path, const boost::function<void (const std::stringstream&)>& json_cb)
  : resolver_(io_service),
  socket_(io_service),
  json_cb_(json_cb)
{
  // Form the request. We specify the "Connection: close" header so that the
  // server will close the socket after transmitting the response. This will
  // allow us to treat all data up until the EOF as the content.
  std::ostream request_stream(&request_);
  request_stream << "GET " << path << " HTTP/1.0\r\n";
  request_stream << "Host: " << server << "\r\n";
  request_stream << "Accept: */*\r\n";
  request_stream << "Connection: close\r\n\r\n";

  // Start an asynchronous resolve to translate the server and service names
  // into a list of endpoints.
  tcp::resolver::query query(server, "http");
  resolver_.async_resolve(query,
              boost::bind(&http_client::handle_resolve, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::iterator));
  boost::thread([this]{
    io_service.run();
    delete this;
  });
  
}
void http_client::handle_resolve(const boost::system::error_code& err,
          tcp::resolver::iterator endpoint_iterator)
{
  if (!err)
  {
    // Attempt a connection to the first endpoint in the list. Each endpoint
    // will be tried until we successfully establish a connection.
    tcp::endpoint endpoint = *endpoint_iterator;
    socket_.async_connect(endpoint,
                boost::bind(&http_client::handle_connect, this,
                      boost::asio::placeholders::error, ++endpoint_iterator));
  }
  else
  {
    std::cout << "Error: " << err.message() << "\n";
  }
}

void http_client::handle_connect(const boost::system::error_code& err,
          tcp::resolver::iterator endpoint_iterator)
{
  if (!err)
  {
    // The connection was successful. Send the request.
    boost::asio::async_write(socket_, request_,
                 boost::bind(&http_client::handle_write_request, this,
                       boost::asio::placeholders::error));
  }
  else if (endpoint_iterator != tcp::resolver::iterator())
  {
    // The connection failed. Try the next endpoint in the list.
    socket_.close();
    tcp::endpoint endpoint = *endpoint_iterator;
    socket_.async_connect(endpoint,
                boost::bind(&http_client::handle_connect, this,
                      boost::asio::placeholders::error, ++endpoint_iterator));
  }
  else
  {
    std::cout << "Error: " << err.message() << "\n";
  }
}

void http_client::handle_write_request(const boost::system::error_code& err)
{
  if (!err)
  {
    // Read the response status line.
    boost::asio::async_read_until(socket_, response_, "\r\n",
                    boost::bind(&http_client::handle_read_status_line, this,
                          boost::asio::placeholders::error));
  }
  else
  {
    std::cout << "Error: " << err.message() << "\n";
  }
}

void http_client::handle_read_status_line(const boost::system::error_code& err)
{
  if (!err)
  {
    // Check that response is OK.
    std::istream response_stream(&response_);
    std::string http_version;
    response_stream >> http_version;
    unsigned int status_code;
    response_stream >> status_code;
    std::string status_message;
    std::getline(response_stream, status_message);
    if (!response_stream || http_version.substr(0, 5) != "HTTP/")
    {
      //std::cout << "Invalid response\n";
      return;
    }
    if (status_code != 200)
    {
      //std::cout << "Response returned with status code ";
      //std::cout << status_code << "\n";
      return;
    }

    // Read the response headers, which are terminated by a blank line.
    boost::asio::async_read_until(socket_, response_, "\r\n\r\n",
                    boost::bind(&http_client::handle_read_headers, this,
                          boost::asio::placeholders::error));
  }
  else
  {
    std::cout << "Error: " << err << "\n";
  }
}

void http_client::handle_read_headers(const boost::system::error_code& err)
{
  if (!err)
  {
    // Process the response headers.
    std::istream response_stream(&response_);
    std::string header;
    while (std::getline(response_stream, header) && header != "\r");
      //std::cout << header << "\n";
    //std::cout << "\n";

    // Write whatever content we already have to output.
    if (response_.size() > 0)
      res_ << &response_;

    // Start reading remaining data until EOF.
    boost::asio::async_read(socket_, response_,
                boost::bind(&http_client::handle_read_content, this,
                      boost::asio::placeholders::error));
  }
  else
  {
    std::cout << "Error: " << err << "\n";
  }
}

void http_client::handle_read_content(const boost::system::error_code& err)
{
  if (!err)
  {
    // Write all of the data that has been read so far.
    res_ << &response_;
    // Continue reading remaining data until EOF.
    boost::asio::async_read(socket_, response_,
                boost::bind(&http_client::handle_read_content, this,
                      boost::asio::placeholders::error));
  }
  else if (err != boost::asio::error::eof)
  {
    std::cout << "Error: " << err << "\n";
  }
  else
  {
    json_cb_(res_);
    io_service.post([this]() {
      socket_.close();
      io_service.stop();
    });
    return;
  }
}
