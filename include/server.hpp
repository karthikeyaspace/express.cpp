// /include/server.hpp

#pragma once

#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <unordered_map>
#include <functional>

#include "config.hpp"

/**
 * TCP socket implementation for HTTP server
 */

namespace http_server {

  class HttpServer {
    public: 
      HttpServer(const server_configuration &config);
      ~HttpServer();

      void get(const std::string& path, std::function<response_t(request_t)> handler) {
        routes["GET"][path] = handler;
      }
      void post(const std::string& path, std::function<response_t(request_t)> handler) {
        routes["POST"][path] = handler;
      }
      void put(const std::string& path, std::function<response_t(request_t)> handler) {
        routes["PUT"][path] = handler;
      }
      void delete_route(const std::string& path, std::function<response_t(request_t)> handler) {
        routes["DELETE"][path] = handler;
      }

      void start();
      
    private:
      // ["Method"]["/path"] = handler
      std::unordered_map<std::string, std::unordered_map<std::string, std::function<response_t(request_t)>>> routes;

      server_configuration config;

      void setupServer();
      void acceptConnections();
      void handleClient(int client_fd);
      
  };
} // namespace http_server