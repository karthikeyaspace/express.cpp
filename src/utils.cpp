// /src/utils.cpp

#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <ctime>
#include <iomanip>

#include "config.hpp"
#include "utils.hpp"

namespace http_server {

  request_t parse(const std::string &request) {
    request_t req_obj;
    std::istringstream request_stream(request);
    std::string line;

    // Parse request line
    std::getline(request_stream, line);
    std::istringstream request_line(line);
    request_line >> req_obj.method >> req_obj.path >> req_obj.version;

    // Parse headers
    while (std::getline(request_stream, line) && line != "\r") {
        auto colon_pos = line.find(':');
        if (colon_pos != std::string::npos) {
            std::string key = line.substr(0, colon_pos);
            std::string value = line.substr(colon_pos + 1);
            req_obj.headers[key] = value;
        }
    }


    // Parse body
    int content_length = 0;
    if (req_obj.headers.find("Content-Length") != req_obj.headers.end()) {
        content_length = std::stoi(req_obj.headers["Content-Length"]);
    }
    if (content_length > 0) {
        std::string body;
        body.resize(content_length);
        request_stream.read(&body[0], content_length);
        req_obj.body = body;
    }

    return (request_t)req_obj;
  }

  std::string get_time() {
    std::time_t now = std::time(nullptr);
    std::tm *tm_now = std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(tm_now, "%Y-%m-%d %H:%M:%S");
    std::string timestamp = oss.str();
    return "[" + timestamp + "] ";
  }
}