#ifndef SERVER_HTTP_H
#define SERVER_HTTP_H

#include "server_http.hpp"

class HttpServer : public SimpleWeb::Server<SimpleWeb::HTTP>
{
public:
  HttpServer(unsigned short port, size_t num_threads=1, long timeout_request=5, long timeout_content=300);
  virtual ~HttpServer() {}
};

#endif
