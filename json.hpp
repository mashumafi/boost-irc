#include <iostream>
#include <string>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

void json(std::string host, std::string path, const boost::function<void (const boost::property_tree::ptree& pt)>& json_result)
{
  using boost::asio::ip::tcp;
  
  boost::asio::io_service* io_service = new boost::asio::io_service();
  std::string* pHost = new std::string(host);
  std::string* pPath = new std::string(path);

  // Get a list of endpoints corresponding to the server name.
  tcp::resolver resolver(*io_service);
  tcp::resolver::query query(host, "http");
  tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

  // Try each endpoint until we successfully establish a connection.
  tcp::socket* socket = new tcp::socket(*io_service);
  boost::asio::async_connect(*socket, endpoint_iterator, [io_service, pHost, pPath, socket, json_result] (boost::system::error_code ec, tcp::resolver::iterator)
  {
    // Form the request. We specify the "Connection: close" header so that the
    // server will close the socket after transmitting the response. This will
    // allow us to treat all data up until the EOF as the content.
    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream << "GET " << *pPath << " HTTP/1.0\r\n";
    request_stream << "Host: " << *pHost << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";
    
    delete pHost;
    delete pPath;
  
    // Send the request.
    boost::asio::async_write(*socket, request, [io_service, socket, json_result] (boost::system::error_code ec, std::size_t)
    {
      // Read the response status line. The response streambuf will automatically
      // grow to accommodate the entire line. The growth may be limited by passing
      // a maximum size to the streambuf constructor.
      boost::asio::streambuf* response = new boost::asio::streambuf();
      boost::asio::async_read_until(*socket, *response, "\r\n", [io_service, socket, response, json_result] (boost::system::error_code ec, std::size_t)
      {
        // Check that response is OK.
        std::istream* response_stream = new std::istream(response);
        std::string http_version;
        unsigned int status_code = -1;
        *response_stream >> http_version;
        *response_stream >> status_code;
        std::string status_message;
        std::getline(*response_stream, status_message);
        if (!(*response_stream) || http_version.substr(0, 5) != "HTTP/" || status_code != 200)
        {
          delete response_stream;
          delete response;
          io_service->stop();
          socket->close();
          delete socket;
          return;
        }
      
        // Read the response headers, which are terminated by a blank line.
        boost::asio::async_read_until(*socket, *response, "\r\n\r\n", [io_service, socket, response, response_stream, json_result] (boost::system::error_code ec, std::size_t)
        {
          std::string header;
          while (std::getline(*response_stream, header) && header != "\r");
          std::stringstream ss;
          if (response->size() > 0)
            ss << response;
        
          // Read until EOF, writing data to output as we go.
          boost::system::error_code error;
          while (boost::asio::read(*socket, *response, boost::asio::transfer_at_least(1), error))
            ss << response;
          boost::property_tree::ptree pt;
          boost::property_tree::read_json(ss, pt);
          json_result(pt);
          
          delete response_stream;
          delete response;
          io_service->stop();
          socket->close();
          delete socket;
          
          if (error != boost::asio::error::eof)
            throw boost::system::system_error(error);
        });
      });
    });
  });
  boost::thread t([io_service](){
    io_service->run();
    delete io_service;
  });

  return;
}