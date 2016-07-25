#include "twitch.hpp"

Twitch::Twitch() : IRC("irc.chat.twitch.tv", "6667")
{

}

Twitch::~Twitch()
{

}

void Twitch::join(const std::string& channel, const std::string& keys)
{
  IRC::join("#" + channel, keys);
}

void Twitch::join(const std::vector<std::string>& channel, const std::vector<std::string>& keys)
{
  join(boost::algorithm::join(channel, ",#"), boost::algorithm::join(keys, ","));
}

void Twitch::part(const std::vector<std::string>& channel, const std::string& msg)
{
  join(boost::algorithm::join(channel, ",#"), msg);
}

void Twitch::part(const std::string& channel, const std::string& msg)
{
  IRC::part("#" + channel, msg);
}
