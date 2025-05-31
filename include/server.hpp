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
#include <thread>

#include "config.hpp"
#include "thread_pool.hpp"

/**
 * TCP socket implementation for HTTP server
 */

namespace http_server {

  class HttpServer {
    public: 
      HttpServer(const server_configuration &config);
      ~HttpServer();

      void get(const std::string& path, std::function<void(const request_t&, response_t&)> handler) {
        register_route(path, handler, "GET");
      }
      void post(const std::string& path, std::function<void(const request_t&, response_t&)> handler) {
        register_route(path, handler, "POST");
      }
      void put(const std::string& path, std::function<void(const request_t&, response_t&)> handler) {
        register_route(path, handler, "PUT");
      }
      void delete_route(const std::string& path, std::function<void(const request_t&, response_t&)> handler) {
        register_route(path, handler, "DELETE");
      }

      void serve_static(const std::string& document_root);

      void start();

    private:
      // ["Method"]["/path"] = handler
      // Real systems do no use unordered_map for routing, it uses tries or radix trees or regex matching
      // to support features like route parameters, middlewares
      std::unordered_map<std::string, std::unordered_map<std::string, std::function<void(const request_t&, response_t&)>>> routes;

      server_configuration config;

      std::unique_ptr<ThreadPool> thread_pool;

      void register_route(const std::string &path, 
                          std::function<void(const request_t&, response_t&)> handler,
                          const std::string &method) {
       if (routes[method].count(path) > 0) {
         std::cerr << "Route already exists for " << method << " " << path << std::endl;
         exit(EXIT_FAILURE);
       }
        routes[method][path] = handler;
      }
      
      void setupServer();
      void acceptConnections();
      void helper(const std::string &alpha);
      void handleClient(int client_fd);
      
  };
} // namespace http_server