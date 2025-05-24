// /include/utils.hpp

#pragma once

#include "config.hpp"
#include <string>

namespace http_server{

  request_t parse(const std::string &request);
  std::string get_time();
  void start_log_thread(const std::string &log_file_path);
  void log(const std::string &log_level, const std::string &log_message);
  void stop_log_thread();

} // namespace http_server