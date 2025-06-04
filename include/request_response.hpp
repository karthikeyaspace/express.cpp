// /include/request_response.hpp

#pragma once

#include <string>
#include <unordered_map>
#include <sstream>
#include <unistd.h> 

#include "config.hpp"

namespace express {

  class Request {
  public:
    std::string method;
    std::string path;
    std::string version;
    std::unordered_map<std::string, std::string> headers;
    std::string body;

    Request() = default;

    Request(const std::string &method, const std::string &path, const std::string &version,
            const std::unordered_map<std::string, std::string> &headers,
            const std::string &body)
        : method(method), path(path), version(version), headers(headers), body(body) {}

    std::string to_string() const {
      std::string request = method + " " + path + " " + version + "\r\n";
      for (const auto &header : headers) {
        request += header.first + ": " + header.second + "\r\n";
      }
      request += "\r\n" + body;
      return request;
    }
  };

  class Response {
  public:
    std::string version = "HTTP/1.1";
    int status_code = 200;
    std::string status_message = "OK";
    std::unordered_map<std::string, std::string> headers;
    std::string body;

    Response(int client_fd): client_fd(client_fd) {};

    void set_content_type(const std::string &type) {
      headers["Content-Type"] = type;
    }
    
    void set_header(const std::string &key, const std::string &value) {
      headers[key] = value;
    }

    void status(int code) {
      if (status_codes.find(code) != status_codes.end()) {
        status_message = status_codes.at(code);
      } else {
        status_message = "Unknown";
        WARNING("Unknown status code: " + std::to_string(code));
      }
      status_code = code;
    }

    void message(const std::string &msg) {
      body = msg;
      headers["Content-Length"] = std::to_string(body.size());
      if (headers.find("Content-Type") == headers.end()) {
        headers["Content-Type"] = "text/plain";
      }
      send();
    }

    void json(const std::unordered_map<std::string, std::string>& json_obj) {
      std::ostringstream oss;
      oss << "{";
      for (auto it = json_obj.begin(); it != json_obj.end(); ++it) {
        if (it != json_obj.begin()) oss << ", ";
        oss << "\"" << it->first << "\": \"" << it->second << "\"";
      }
      oss << "}";
      message(oss.str());
      set_content_type("application/json");
      send();
    }

    void redirect(const std::string &location) {
      status(302);
      headers["Location"] = location;
      body = "";
      send();
    }

    void send() {
      if (client_fd == -1) {
        ERROR("Failed to initialize server response");
      }
      std::string res = this->prepare_response();
      write(client_fd, res.c_str(), res.size());
      close(client_fd);
    }

    std::string prepare_response() const {
      std::ostringstream oss;
      oss << version << " " << status_code << " " << status_message << "\r\n";
      for (const auto &header : headers) {
        oss << header.first << ": " << header.second << "\r\n";
      }
      oss << "\r\n" << body;
      return oss.str();
    }

    void not_found() {
      status(404);
      message("Not Found");
      send();
    }

  private:
    int client_fd = -1;
  };

} // namespace express
