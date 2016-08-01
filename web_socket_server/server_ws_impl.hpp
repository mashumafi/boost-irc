#ifndef SERVER_WS_H
#define SERVER_WS_H

#include "server_ws.hpp"

class WsServer : public SimpleWeb::SocketServer<SimpleWeb::WS>
{
public:
  WsServer(unsigned short port, size_t num_threads=1, size_t timeout_request=5, size_t timeout_idle=0);
  virtual ~WsServer() {}
};

#endif
