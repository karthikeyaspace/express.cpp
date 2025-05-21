// /include/config.hpp

#pragma once

#include <cassert>

namespace http_server {

  #define ASSERT(expr, msg) assert((expr) && msg)
  
  static constexpr int PORT = 7000;
  static constexpr int BUFFER_SIZE = 4096;
    
} // namespace http_server
