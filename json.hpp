#include <iostream>
#include <string>

#include <boost/asio.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

void json(std::string host, std::string path, const boost::function<void (const int code, const boost::property_tree::ptree& pt)>& json_result)
{
  using boost::asio::ip::tcp;
  
  boost::asio::io_service io_service;

  // Get a list of endpoints corresponding to the server name.
  tcp::resolver resolver(io_service);
  tcp::resolver::query query(host, "http");
  tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

  // Try each endpoint until we successfully establish a connection.
  tcp::socket socket(io_service);
  boost::asio::async_connect(socket, endpoint_iterator, [=, &socket] (boost::system::error_code ec, tcp::resolver::iterator)
  {
    // Form the request. We specify the "Connection: close" header so that the
    // server will close the socket after transmitting the response. This will
    // allow us to treat all data up until the EOF as the content.
    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream << "GET " << path << " HTTP/1.0\r\n";
    request_stream << "Host: " << host << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";
  
    // Send the request.
    boost::asio::async_write(socket, request, [&socket] (boost::system::error_code ec, std::size_t)
    {
      // Read the response status line. The response streambuf will automatically
      // grow to accommodate the entire line. The growth may be limited by passing
      // a maximum size to the streambuf constructor.
      boost::asio::streambuf response;
      boost::asio::async_read_until(socket, response, "\r\n", [&socket, &response] (boost::system::error_code ec, std::size_t)
      {
        // Check that response is OK.
        std::istream response_stream(&response);
        std::string http_version;
        unsigned int status_code = -1;
        response_stream >> http_version;
        response_stream >> status_code;
        std::string status_message;
        std::getline(response_stream, status_message);
        if (!response_stream || http_version.substr(0, 5) != "HTTP/")
        {
          return;
        }
        if (status_code != 200)
        {
          return;
        }
      
        // Read the response headers, which are terminated by a blank line.
        boost::asio::async_read_until(socket, response, "\r\n\r\n", [&socket, &response, &response_stream] (boost::system::error_code ec, std::size_t)
        {
          std::string header;
          while (std::getline(response_stream, header) && header != "\r");
          std::stringstream ss;
          if (response.size() > 0)
            ss << &response;
        
          // Read until EOF, writing data to output as we go.
          boost::system::error_code error;
          while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error))
            ss << &response;
          boost::property_tree::ptree pt;
          boost::property_tree::read_json(ss, pt);
          
          if (error != boost::asio::error::eof)
            throw boost::system::system_error(error);
        });
      });
    });
  });

  return;
}