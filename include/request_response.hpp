// /include/request_response.hpp

#pragma once

#include <string>
#include <unordered_map>
#include <sstream>

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

    Response() = default;

    void set_content_type(const std::string &type) {
      headers["Content-Type"] = type;
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
    }

    void send(const std::string &data) {
      body = data;
      headers["Content-Length"] = std::to_string(body.size());
      if (headers.find("Content-Type") == headers.end()) {
        headers["Content-Type"] = "text/plain";
      }
    }

    void json(const std::unordered_map<std::string, std::string>& json_obj) {
      std::ostringstream oss;
      oss << "{";
      for (auto it = json_obj.begin(); it != json_obj.end(); ++it) {
        if (it != json_obj.begin()) oss << ", ";
        oss << "\"" << it->first << "\": \"" << it->second << "\"";
      }
      oss << "}";
      send(oss.str());
      set_content_type("application/json");
    }

    void redirect(const std::string &location) {
      status(302);
      headers["Location"] = location;
      body = "";
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
  };

} // namespace express
