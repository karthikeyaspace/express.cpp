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
#include "request_response.hpp"
#include "thread_pool.hpp"
#include "logger.hpp"
#include "utils.hpp"

/**
 * TCP socket implementation for HTTP server
 */

namespace express {

  class HttpServer {
    public: 
      HttpServer(const server_configuration &config);
      ~HttpServer();
      
      void get(const std::string& path, std::function<void(const Request&, Response&)> handler) {
        register_route(path, handler, "GET");
      }
      void post(const std::string& path, std::function<void(const Request&, Response&)> handler) {
        register_route(path, handler, "POST");
      }
      void put(const std::string& path, std::function<void(const Request&, Response&)> handler) {
        register_route(path, handler, "PUT");
      }
      void del(const std::string& path, std::function<void(const Request&, Response&)> handler) {
        register_route(path, handler, "DELETE");
      }
      
      void serve(const std::string& document_root);
      
      void start(std::function<void()> on_start);
      
    private:
      server_configuration config;
      
      // ["Method"]["/path"] = handler
      // Real systems do no use unordered_map for routing, it uses tries or radix trees or regex matching 
      // to support features like route parameters, middlewares
      std::unordered_map<std::string, std::unordered_map<std::string, std::function<void(const Request&, Response&)>>> routes;

      // TODO:
      // refactor to routes ds to support middlewares, route parameters, regex matching
      // key value pair, key will be combination of method and path - e.g. "GET /api/users"
      // value will be handler chain - linked list of functions 
      // first will be middlewares of path *, then middlewares of that path, then actual handler of that path

      
      // why unique_ptr?
      // Because we want to manage the lifetime of these objects automatically, and they get cleaned when HttpServer is destroyed
      std::unique_ptr<ThreadPool> thread_pool;
      std::unique_ptr<Logger> logger;

      void register_route(const std::string &path, 
                          std::function<void(const Request&, Response&)> handler,
                          const std::string &method) {
       if (routes[method].count(path) > 0) {
         std::cerr << "Route already exists for " << method << " " << path << std::endl;
         exit(EXIT_FAILURE);
       }
        routes[method][path] = handler;
      }
      
      void setupServer(std::function<void()> on_start);
      void acceptConnections();
      void helper(const std::string &alpha);
      void handleClient(int client_fd);
      void stop();
  };
} // namespace express