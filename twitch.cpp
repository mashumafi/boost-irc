#include "twitch.hpp"

Twitch::Twitch() : IRC("irc.chat.twitch.tv", "6667")
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
      send("CAP REQ :twitch.tv/tags");
    default:
      std::cout << "> " << msg.raw() << std::endl;
      break;
  }
}
