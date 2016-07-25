#include "irc.hpp"

#include <stdarg.h>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <vector>

#include <boost/thread.hpp>
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
          
          Message msg(line);
          
          try
          {
            int code = stoi(msg.command);
            switch(code)
            {
              case RPL_WELCOME:
              case RPL_YOURHOST:
              case RPL_CREATED:
              case RPL_MYINFO:
              case RPL_MOTDSTART:
              case RPL_MOTD:
                break;
              case RPL_ENDOFMOTD:
                send("CAP REQ :twitch.tv/tags");
                break;
              default:
                std::cout << "> " << msg.raw << std::endl;
                break;
            }
          }
          catch(std::invalid_argument)
          {
            switch(hashit(msg.command))
            {
              case PRIVMSG:
                privmsged(msg);
                break;
              case PING:
                send("PONG", list_of(":" + msg.params[0]));
                break;
              case NONE:
              default:
                std::cout << "> " << msg.raw << std::endl;
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

void IRC::login(const std::string& nick, const std::string& pass)
{
  this->pass(pass);
  this->nick(nick);
}

void IRC::send(const std::string& cmd, const std::vector<std::string>& vec)
{
  std::string raw = str(boost::format("%1% %2%") % cmd % boost::algorithm::join(vec, " "));
  send(raw);
}

void IRC::send(const std::string& raw)
{
  std::string msg = raw + "\r\n";
  std::cout << "< " << msg;
  boost::asio::write(*s, boost::asio::buffer(msg.c_str(), msg.length()));
}

void IRC::pass(const std::string& pwd)
{
  send("PASS", list_of(pwd));
}

void IRC::nick(const std::string& pwd)
{
  send("NICK", list_of(pwd));
}

void IRC::quit(const std::string& pwd)
{
  send("QUIT", list_of(pwd));
}

void IRC::join(const std::vector<std::string>& channel, const std::vector<std::string>& keys)
{
  join(boost::algorithm::join(channel, ","), boost::algorithm::join(keys, ","));
}

void IRC::join(const std::string& channel, const std::string& keys)
{
  send("JOIN", list_of(channel)(keys));
}

void IRC::join0()
{
  send("JOIN", list_of("0"));
}

void IRC::joined(const Message&)
{
  
}

void IRC::part(const std::vector<std::string>& channel, const std::string& msg)
{
  part(boost::algorithm::join(channel, ","), msg);
}

void IRC::part(const std::string& channel, const std::string& msg)
{
  send("PART", list_of(channel)(msg));
}

void IRC::privmsg(const std::string& msgtarget, const std::string& text_to_be_sent)
{
  send("PRIVMSG", list_of(msgtarget)(text_to_be_sent));
}

void IRC::privmsged(const Message& msg)
{
  std::cout << "> " << msg.nickname << ": " << msg.params[1] << std::endl;
}

Reply hashit(const std::string& inString)
{
  if (inString == "PRIVMSG") return PRIVMSG;
  if (inString == "PING") return PING;
  return NONE;
}
