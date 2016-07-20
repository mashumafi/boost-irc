#ifndef TWITCH_H
#define TWITCH_H

#include "irc.hpp"

class Twitch : public IRC
{
public:
  Twitch();
  virtual ~Twitch();
};

#endif