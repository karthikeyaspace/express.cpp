// /include/logger.hpp


#pragma once

#include "config.hpp"


// TODO: class Logger

namespace http_server {
  void start_log_thread(const std::string &log_file_path);
  
  void log(const std::string &log_level, const std::string &log_message);
  
  void stop_log_thread();
} // namespace http_server