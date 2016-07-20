#include "irc.hpp"

#include <stdarg.h>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <vector>

#include <boost/regex.hpp>
#include <boost/thread.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/assign/list_of.hpp>

using namespace boost::assign;

enum { max_length = 32 };

IRC::IRC(std::string host, std::string port)
{
  try
  {
    boost::asio::io_service io_service;
  
    tcp::resolver resolver(io_service);
    tcp::resolver::query query(tcp::v4(), host, port);
    tcp::resolver::iterator iterator = resolver.resolve(query);
  
    s = new tcp::socket(io_service);
    s->connect(*iterator);
    
    boost::thread t([this]
    {
      std::string buff;
      char reply[max_length];
      size_t reply_length;
      do
      {
        reply_length = boost::asio::read(*s, boost::asio::buffer(reply, max_length));
        buff += reply;
        int idx;
        while((idx = buff.find("\r\n")) > 0)
        {
          std::string line = buff.substr(0, idx);
          std::stringstream data;
          data << line;
          Response response;
          response.raw = line;
          data >> response.host;
          data >> response.code;
          data >> response.username;
          data.ignore(2);
          getline(data, response.message);
          try
          {
            int code = stoi(response.code);
            switch(code)
            {
              case RPL_WELCOME:
              case RPL_YOURHOST:
              case RPL_CREATED:
              case RPL_MYINFO:
              case RPL_MOTDSTART:
              case RPL_MOTD:
              case RPL_ENDOFMOTD:
                break;
              default:
                std::cout << "> " << response.raw << std::endl;
                break;
            }
          }
          catch(std::invalid_argument)
          {
            switch(hashit(response.code))
            {
              case PRIVMSG:
                break;
              case NONE:
              default:
                std::cout << "> " << response.raw << std::endl;
                break;
            }
          }
          buff = buff.substr(idx + 2);
        }
        
      } while(reply_length > 0);
      std::cout << "ended!";
    });
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
}

IRC::~IRC()
{
  
}

void IRC::login(const std::string nick, const std::string pass)
{
  send("PASS", list_of(pass));
  send("NICK", list_of(nick));
}

void IRC::send(const std::string cmd, const std::vector<std::string>& vec)
{
  std::string raw = str(boost::format("%1% %2%") % cmd % boost::algorithm::join(vec, " "));
  send(raw);
}

void IRC::send(std::string raw)
{
  raw += "\r\n";
  std::cout << "< " << raw;
  boost::asio::write(*s, boost::asio::buffer(raw.c_str(), raw.length()));
}
