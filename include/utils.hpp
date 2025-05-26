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
  
  std::vector<std::string> get_files_in_directory(const std::string &directory);
  
  std::string get_path(const std::string &file_name);
  
  std::string get_file_mimetype(const std::string &file_name);

  std::string read_file(const std::string &file_path);

} // namespace http_server