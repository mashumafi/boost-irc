#ifndef IRC_H
#define IRC_H

#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

enum Reply
{
  ERR_NOSUCHNICK = 401,
  ERR_NOSUCHSERVER = 402,
  ERR_NOSUCHCHANNEL = 403
};

class Response
{
public:
  std::string raw;
  std::string host;
  boost::uint16_t code;
  std::string username;
  std::string message;
};

class IRC
{
public:
  IRC(std::string, std::string);
  virtual ~IRC();
  virtual void login(const std::string, const std::string);
  virtual void send(const std::string, const std::vector<std::string>&);
  virtual void send(std::string);
private:
  tcp::socket* s;
};

#endif
