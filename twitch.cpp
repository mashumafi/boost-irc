#include "twitch.hpp"

Twitch::Twitch() : IRC("irc.chat.twitch.tv", "6667")
{

}

Twitch::~Twitch()
{

}

void Twitch::join(std::string channel, std::string key)
{
  IRC::join("#" + channel, key);
}
