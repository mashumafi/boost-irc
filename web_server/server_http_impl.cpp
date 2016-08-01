#include "server_http_impl.hpp"

HttpServer::HttpServer(unsigned short port, size_t num_threads, long timeout_request, long timeout_content)
  : SimpleWeb::Server<SimpleWeb::HTTP>(port, num_threads, timeout_request, timeout_content)
{
}
