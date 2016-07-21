#ifndef TWITCH_H
#define TWITCH_H

#include "irc.hpp"

class Twitch : public IRC
{
public:
  Twitch();
  virtual ~Twitch();
  virtual void join(std::string, std::string="") override;
};

#endif
