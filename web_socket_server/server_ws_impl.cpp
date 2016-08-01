#include "server_ws_impl.hpp"

WsServer::WsServer(unsigned short port, size_t num_threads, size_t timeout_request, size_t timeout_idle)
  : SimpleWeb::SocketServer<SimpleWeb::WS>(port, num_threads, timeout_request, timeout_idle)
{
}
