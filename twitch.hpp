#ifndef TWITCH_H
#define TWITCH_H

#include "irc.hpp"

class Twitch : public IRC
{
public:
  Twitch();
  virtual ~Twitch();
  
  virtual void join(const std::string&, const std::string& keys="") override;
  virtual void join(const std::vector<std::string>&, const std::vector<std::string>& keys=std::vector<std::string>()) override;
  
  virtual void part(const std::vector<std::string>&, const std::string& msg="") override;
  virtual void part(const std::string&, const std::string& msg="") override;
};

#endif
