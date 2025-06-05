// /src/utils.cpp

#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <ctime>
#include <iomanip>
#include <vector>
#include <dirent.h>
#include <fstream>
#include <unistd.h>
#include <arpa/inet.h>

#include "utils.hpp"
#include "config.hpp"

namespace express{

  Request parse(const std::string &raw_request){
    std::istringstream request_stream(raw_request);
    std::string line;
    std::string path;

    Request req;

    // Parse request line
    std::getline(request_stream, line);
    std::istringstream request_line(line);
    request_line >> req.method >> path >> req.version;

    parse_path(path, req);

    // Parse headers
    while (std::getline(request_stream, line) && line != "\r" && !line.empty()){
      auto colon_pos = line.find(':');
      if (colon_pos != std::string::npos){
        std::string key = line.substr(0, colon_pos);
        std::string value = line.substr(colon_pos + 1);
        value.erase(0, value.find_first_not_of(" ")); 
        req.headers[key] = value;
      }
    }

    // Parse body
    if (req.headers.count("Content-Length")) {
      int len = std::stoi(req.headers["Content-Length"]);
      std::string body(len, '\0');
      request_stream.read(&body[0], len);
      req.body = body;
    }

    return req;
  }

  // parsing path for query parameters
  void parse_path(const std::string &path, Request &req) {  
    size_t query_pos = path.find('?');
    if (query_pos != std::string::npos) {
      req.path = path.substr(0, query_pos);
      std::string query_string = path.substr(query_pos + 1);
      std::istringstream query_stream(query_string);
      std::string param;

      while(std::getline(query_stream, param, '&')) {
        size_t equal_pos = param.find('=');
        if (equal_pos != std::string::npos) {
          std::string key = param.substr(0, equal_pos);
          std::string value = param.substr(equal_pos + 1);
          req.params[key] = value;
        }
      }
    } else req.path = path;
  }

  std::string get_time(){
    std::time_t now = std::time(nullptr);
    std::tm *tm_now = std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(tm_now, "%Y-%m-%d %H:%M:%S");
    std::string timestamp = oss.str();
    return "[" + timestamp + "] ";
  }

  std::string get_client_ip(int client_fd)
  {
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    if (getpeername(client_fd, (struct sockaddr *)&client_addr, &addr_len) == -1)
    {
      perror("getpeername failed");
      return "";
    }
    char ip_str[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &client_addr.sin_addr, ip_str, sizeof(ip_str)) == nullptr)
    {
      perror("inet_ntop failed");
      return "";
    }
    return std::string(ip_str);
  }

  std::vector<std::string> get_files_in_directory(const std::string &directory)
  {
    std::vector<std::string> files;
    DIR *dir = opendir(directory.c_str());
    if (!dir)
    {
      ERROR("Failed to open directory: ");
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr)
    {
      if (entry->d_type == DT_REG)
      {
        files.push_back(entry->d_name);
      }
    }
    closedir(dir);
    return files;
  }

  std::string read_file(const std::string &filepath)
  {
    std::ifstream file(filepath, std::ios::in | std::ios::binary);

    if (!file)
    {
      ERROR("Failed to open file: " + filepath);
    }

    std::ostringstream oss;
    oss << file.rdbuf();
    return oss.str();
  }

  std::string get_path(const std::string &filename)
  {
    std::string ext = filename.substr(filename.find_last_of('.'));
    if (filename == "index.html")
      return "/";
    else if (ext == ".html")
      return "/" + filename.substr(0, filename.find_last_of('.'));
    else
      return "/" + filename;
  }

  std::string get_file_mimetype(const std::string &filename)
  {
    std::string extension = filename.substr(filename.find_last_of('.') + 1);
    if (extension == "html")
      return "text/html";
    else if (extension == "css")
      return "text/css";
    else if (extension == "js")
      return "application/javascript";
    return "text/html";
  }
}