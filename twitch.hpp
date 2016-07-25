#ifndef TWITCH_H
#define TWITCH_H

#include "irc.hpp"

class Twitch : public IRC
{
public:
  Twitch();
  virtual ~Twitch();
protected:
  virtual void reply(const Message&, const Reply code) override;
};

#endif
