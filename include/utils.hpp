#pragma once

#include "config.hpp"
#include <string>

namespace http_server {
  request_t parse(const std::string &request);

} // namespace http_server