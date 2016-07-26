#ifndef TWITCH_H
#define TWITCH_H

#include "irc.hpp"

class Twitch : public IRC
{
public:
  Twitch(const std::string& nick, const std::string& pass);
  virtual ~Twitch();
protected:
  virtual void reply(const Message&, const Reply code) override;
  virtual void privmsg(const Message&, const std::string&, const std::string&) override;
};

#endif
