#include "irc.hpp"

#include <stdarg.h>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <vector>

#include <boost/thread.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/algorithm/string.hpp>

using namespace boost::assign;

IRC::IRC(const std::string& host, const std::string& port, const std::string& nick, const std::string& pass) : m_nick(nick), m_pass(pass)
{
  try
  {
    tcp::resolver resolver(io_service);
    tcp::resolver::query query(tcp::v4(), host, port);
    tcp::resolver::iterator iterator = resolver.resolve(query);
  
    s = new tcp::socket(io_service);
    connect(iterator);
    boost::thread t([this](){ io_service.run(); });
    
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
  send(str(boost::format("%1% %2%") % cmd % boost::algorithm::join(vec, " ")));
}

void IRC::send(const std::string& raw)
{
  std::string* msg = new std::string(raw + "\r\n");
  boost::asio::async_write(*s, boost::asio::buffer(msg->c_str(), msg->length()), [msg] (boost::system::error_code ec, std::size_t)
  {
    std::cout << "< " << *msg;
    delete msg;
  });
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

void IRC::join(const Message& msg, const std::vector<std::string>& channels, const std::vector<std::string>& keys)
{
  std::cout << boost::format("%1% joined %2%") % msg.nickname % boost::algorithm::join(channels, ",") << std::endl;
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
  if (inString == "JOIN") return JOIN;
  return NONE;
}

void IRC::connect(tcp::resolver::iterator endpoint_iterator)
{
  boost::asio::async_connect(*s, endpoint_iterator,
    [this](boost::system::error_code ec, tcp::resolver::iterator)
    {
      if (!ec)
      {
        read();
        connected();
      }
      else
      {
      }
    });
}

void IRC::read()
{
  boost::asio::async_read_until(*s, response, "\r\n",
    [this](boost::system::error_code ec, std::size_t)
    {
      if (!ec)
      {
        std::istream response_stream(&response);
        std::string line;
        do
        {
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
              {
                privmsg(msg, msg.params[0], msg.params[1]);
                break;
              }
              case PING:
              {
                if(msg.params.size() == 1)
                {
                  ping(msg, msg.params[0]);
                }
                else if(msg.params.size() > 1)
                {
                  ping(msg, msg.params[0], msg.params[1]);
                }
                break;
              }
              case JOIN:
              {
                std::vector<std::string> channels;
                std::vector<std::string> keys;
                if(msg.params.size() > 0)
                {
                  boost::split(channels, msg.params[0], boost::is_any_of(","));
                }
                if(msg.params.size() > 1)
                {
                  boost::split(keys, msg.params[1], boost::is_any_of(","));
                }
                join(msg, channels, keys);
                break;
              }
              case NONE:
              default:
              {
                std::cout << "> " << msg.raw() << std::endl;
                break;
              }
            }
          }
        } while(response.size() > 0);
        read();
      }
      else
      {
        s->close();
      }
    });
}

void IRC::connected()
{
  pass(m_pass);
  nick(m_nick);
}

void IRC::close()
{
  io_service.post([this]() { s->close(); });
}
