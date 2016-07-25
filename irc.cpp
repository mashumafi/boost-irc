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

IRC::IRC(const std::string& host, const std::string& port)
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
      size_t reply_length;
      do
      {
        boost::asio::streambuf response;
        reply_length = boost::asio::read_until(*s, response, "\r\n");
        std::istream response_stream(&response);
        std::string line;
        getline(response_stream, line);
        
        Message msg(line);
        
        try
        {
          this->reply(msg, static_cast<Reply>(stoi(msg.command)));
        }
        catch(std::invalid_argument)
        {
          switch(hashit(msg.command))
          {
            case PRIVMSG:
              privmsg(msg, msg.params[0], msg.params[1]);
              break;
            case PING:
              if(msg.params.size() == 1)
              {
                ping(msg, msg.params[0]);
              }
              else if(msg.params.size() > 1)
              {
                ping(msg, msg.params[0], msg.params[1]);
              }
              break;
            case NONE:
            default:
              std::cout << "> " << msg.raw() << std::endl;
              break;
          }
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

void IRC::reply(const Message& msg, const Reply code)
{
}

void IRC::nick(const std::string& pwd)
{
  send("NICK", list_of(pwd));
}

void IRC::quit(const std::string& msg)
{
  send("QUIT", list_of(msg));
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

void IRC::join(const Message& msg)
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

void IRC::part(const Message&)
{
}


void IRC::mode(const std::string& channel, const std::string&, const std::string& modes, const std::string& modeparams)
{
  
}

void IRC::topic(const std::string& channel, const std::string& topic)
{
  
}

void IRC::names(const std::vector<std::string>& channels, const std::string& target)
{
  
}

void IRC::names(const std::string& channel, const std::string& target)
{
  
}

void IRC::list(const std::vector<std::string>& channels, const std::string& target)
{
  
}

void IRC::list(const std::string& channel, const std::string& target)
{
  
}

void IRC::invite(const std::string& nickname, const std::string& channel)
{
  
}

void IRC::kick(const std::vector<std::string>& channels, const std::vector<std::string>& users, const std::string& comment)
{
  
}

void IRC::privmsg(const std::string& msgtarget, const std::string& text_to_be_sent)
{
  send("PRIVMSG", list_of(msgtarget)(":" + text_to_be_sent));
}

void IRC::privmsg(const Message& msg, const std::string& msgtarget, const std::string& text_to_be_sent)
{
  std::cout << msgtarget << "> " << msg.nickname << ": " << text_to_be_sent << std::endl;
}

void IRC::notice(const std::string& msgtarget, const std::string& text)
{
  
}

void IRC::motd(const std::string& target)
{
  
}

void IRC::lusers(const std::string& mask, const std::string& target)
{
  
}

void IRC::version(const std::string& target)
{
  
}

void IRC::stats(const std::string& query, const std::string& target)
{
  
}

void IRC::links(const std::string& remote_server, const std::string& server_mask)
{
  
}

void IRC::time(const std::string& target)
{
  
}

void IRC::connect(const std::string& target_server, const std::string& port, const std::string& remote_server)
{
  
}

void IRC::trace(const std::string& target)
{
  
}

void IRC::admin(const std::string& target)
{
  
}

void IRC::info(const std::string& target)
{
  
}

void IRC::kill()
{
  
}

void IRC::ping(const std::string& server, const std::string& server2)
{
  send("PING", list_of(server)(server2));
}

void IRC::ping(const Message& msg, const std::string& server, const std::string& server2)
{
  pong(server, server2);
}

void IRC::pong(const std::string& server, const std::string& server2)
{
  send("PONG", list_of(server)(server2));
}

void IRC::error(const std::string& error_message)
{
  
}

Reply hashit(const std::string& inString)
{
  if (inString == "PRIVMSG") return PRIVMSG;
  if (inString == "PING") return PING;
  return NONE;
}
