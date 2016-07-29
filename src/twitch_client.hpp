#ifndef TWITCH_H
#define TWITCH_H

#include <irc_client.hpp>

class twitch_client : public irc_client
{
public:
  twitch_client(const std::string& nick, const std::string& pass);
  virtual ~twitch_client();
protected:
  virtual bool reply(const Message&, const Reply code) override;
  virtual void privmsg(const Message&, const std::string&, const std::string&) override;
};

#endif
