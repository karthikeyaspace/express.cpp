// /include/config.hpp

#pragma once

#include <cassert>
#include <unordered_map>


namespace http_server {

  #define ASSERT(expr, msg) assert((expr) && msg)
  
  static constexpr int PORT = 42069;
  static constexpr int BUFFER_SIZE = 4096;

  struct request_t {
    std::string method;
    std::string path;
    std::string version;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
  };

  struct response_t {
    std::string version = "HTTP/1.1";
    int status_code;
    std::string status_message = "ok";
    std::unordered_map<std::string, std::string> headers;
    std::string body;

    std::string to_string() {
      std::string response = version + " " + std::to_string(status_code) + " " + status_message + "\r\n";
      for (const auto& header : headers) {
        response += header.first + ": " + header.second + "\r\n";
      }
      response += "\r\n" + body;
      return response;
    }
  };
  
    
} // namespace http_server
