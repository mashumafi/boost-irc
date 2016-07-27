#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <iostream>
#include <boost/asio.hpp>
#include <boost/function.hpp>

class http_client
{
public:
  static http_client* create(const std::string& server, const std::string& path, const boost::function<void (const std::stringstream&)>& json_cb);
  ~http_client();
private:
  
  http_client(const std::string& server, const std::string& path, const boost::function<void (const std::stringstream&)>& json_cb);
  void handle_resolve(const boost::system::error_code& err, boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

  void handle_connect(const boost::system::error_code& err, boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

  void handle_write_request(const boost::system::error_code& err);

  void handle_read_status_line(const boost::system::error_code& err);

  void handle_read_headers(const boost::system::error_code& err);

  void handle_read_content(const boost::system::error_code& err);

  boost::asio::io_service io_service;
  boost::asio::ip::tcp::resolver resolver_;
  boost::asio::ip::tcp::socket socket_;
  boost::asio::streambuf request_;
  boost::asio::streambuf response_;
  std::stringstream res_;
  boost::function<void (const std::stringstream&)> json_cb_;
};

#endif