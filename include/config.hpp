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

  #define WARNING(msg) { \
    std::cerr << "[WARNING] " << msg << std::endl; \
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
    int threads = 4;
    int rate_limit = 100;
    std::string document_root = "./public";
    std::string log_file_path = "";
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
    int status_code = 200;
    std::string status_message = "OK";
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
    
    std::string prepare_response() {
      if (headers.find("Content-Type") == headers.end()) {
        headers["Content-Type"] = "text/plain";
      }
      if (headers.find("Content-Length") == headers.end()) {
        headers["Content-Length"] = std::to_string(body.size());
      }
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

    void status(int code) {
      assert(status_codes.count(code) > 0);
      status_code = code;
      status_message = status_codes.at(code);
    }
    
    void set_header(const std::string& key, const std::string& value) {
      headers[key] = value;
    }

    void json(const std::unordered_map<std::string, std::string>& json_obj) {
      headers["Content-Type"] = "application/json";
      body = "{";
      for (const auto& [key, value] : json_obj) {
        body += "\"" + key + "\": \"" + value + "\", ";
      }
      body.pop_back();
      body.pop_back();
      body += "}";
    }

    void message(const std::string& msg) {
      headers["Content-Type"] = "text/plain";
      body = msg;
    }
  };
} // namespace http_server
