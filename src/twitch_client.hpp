#ifndef TWITCH_H
#define TWITCH_H

#include <irc_client.hpp>
#include <server_ws_impl.hpp>

class twitch_client : public irc_client
{
public:
  twitch_client(const std::string& nick, const std::string& pass, const WsServer*, const std::shared_ptr<WsServer::Connection>);
  virtual ~twitch_client();
protected:
  virtual bool reply(const Message&, const Reply code) override;
  virtual void privmsg(const Message&, const std::string&, const std::string&) override;
private:
  const WsServer* m_websocket;
  const std::shared_ptr<WsServer::Connection> m_connection;
};

#endif
