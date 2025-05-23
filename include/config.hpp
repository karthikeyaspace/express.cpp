// /include/config.hpp

#pragma once

#include <cassert>
#include <unordered_map>


namespace http_server {

  #define EXIT(msg) { \
    perror(msg); \
    exit(EXIT_FAILURE); \
    close(config.server_fd); \
  }
  
  static constexpr int PORT = 42069;
  static constexpr int BUFFER_SIZE = 4096;
  static constexpr int MAX_THREAD_QUEUE_SIZE = 100;

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

  enum class http_status_code {
    OK = 200,
    CREATED = 201,
    ACCEPTED = 202,
    NO_CONTENT = 204,
    MOVED_PERMANENTLY = 301,
    FOUND = 302,
    NOT_MODIFIED = 304,
    BAD_REQUEST = 400,
    UNAUTHORIZED = 401,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    INTERNAL_SERVER_ERROR = 500
  };
  
  struct server_configuration {
    int server_fd = -1;
    int port = PORT;
    int max_connections = 100;
    int threads = 4;
    int rate_limit = 100;
    std::string document_root = "./public";
  };

  struct request_t {
    std::string method;
    std::string path;
    std::string version;
    std::unordered_map<std::string, std::string> headers;
    std::string body;

    std::string to_string() {
      std::string request = method + " " + path + " " + version + "\r\n";
      for (const auto& header : headers) {
        request += header.first + ": " + header.second + "\r\n";
      }
      request += "\r\n" + body;
      return request;
    }
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

    void set_content_type(const std::string& type) {
      headers["Content-Type"] = type;
    }
  };
  
    
} // namespace http_server
