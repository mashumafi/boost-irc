#include "twitch.hpp"
#include "json.hpp"

Twitch::Twitch(const std::string& nick, const std::string& pass) : IRC("irc.chat.twitch.tv", "6667", nick, pass)
{
  
}

Twitch::~Twitch()
{

}

void Twitch::reply(const Message& msg, const Reply code)
{
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
    {
      send("CAP REQ :twitch.tv/tags");
      json("tmi.twitch.tv", "/group/user/voyboy/chatters", [] (const boost::property_tree::ptree& res)
      {
        boost::property_tree::ptree chatter_count = res.get_child("chatter_count");
        std::cout << chatter_count.get_value<int>() << std::endl;
      });
      break;
    }
    default:
      std::cout << "> " << msg.raw() << std::endl;
      break;
  }
}

void Twitch::privmsg(const Message& msg, const std::string& msgtarget, const std::string& text_to_be_sent)
{
  IRC::privmsg(msg, msgtarget, text_to_be_sent);
}
