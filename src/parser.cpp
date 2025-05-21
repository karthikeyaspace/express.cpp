// /src/parser.cpp

#include <iostream>
#include <sstream>
#include <string>

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
    if (request_stream.peek() != EOF) {
        std::getline(request_stream, req_obj.body);
    }

    return (request_t)req_obj;
  }
}