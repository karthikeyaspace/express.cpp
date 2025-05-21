#pragma once

#include <iostream>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

/**
 * TCP socket implementation for HTTP server
 */

namespace http_server {
  class HttpServer {
    public: 
      HttpServer(int port);
      ~HttpServer();
      void start();

    private:
      int server_fd; // File descriptor for the server socket
      int port;

      void setupServer();
      void acceptConnections();
      
  };
} // namespace http_server