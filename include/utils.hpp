// /include/utils.hpp

#pragma once

#include "config.hpp"
#include <string>

namespace http_server{

  request_t parse(const std::string &request);
  
  std::string get_time();
  
  std::string get_client_ip(int client_fd);

  
  std::vector<std::string> get_files_in_directory(const std::string &directory);
  
  std::string get_path(const std::string &file_name);
  
  std::string get_file_mimetype(const std::string &file_name);

  std::string read_file(const std::string &file_path);

} // namespace http_server