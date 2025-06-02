// /include/config.hpp

#pragma once


#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream>

namespace express {

  // Macros
  #define EXIT(msg) { \
    perror(msg); \
    exit(EXIT_FAILURE); \
  }

  #define ERROR(msg) { \
    std::cerr << "[ERROR] " << msg << std::endl; \
    exit(EXIT_FAILURE); \
  }

  #define WARNING(msg) { \
    std::cerr << "[WARNING] " << msg << std::endl; \
  }

  #define CONSOLE(msg) { \
    std::cout << msg << std::endl; \
  }

  
  static constexpr int BUFFER_SIZE = 4096;
  static constexpr int MAX_QUEUE_SIZE = 1000;
  static constexpr int MAX_BUFFER_QUEUE_SIZE = 100;

  enum class http_method {
    GET,
    POST,
    PUT,
    DELETE,
    PATCH,
    OPTIONS,
    HEAD,
    TRACE,
    CONNECT
  };

  static const std::unordered_map<int, std::string> status_codes = {
    {200, "OK"},
    {201, "Created"},
    {204, "No Content"},
    {301, "Moved Permanently"},
    {302, "Found"},
    {304, "Not Modified"},
    {400, "Bad Request"},
    {401, "Unauthorized"},
    {403, "Forbidden"},
    {404, "Not Found"},
    {500, "Internal Server Error"},
    {501, "Not Implemented"}
  };
  
  struct server_configuration {
    int server_fd = -1;
    int port = 8080;
    int max_connections = 100;
    int threads = 5;
    int rate_limit = 100;
    std::string document_root = "./public";
    std::string log_file_path = "";
  };
  
} // namespace express
