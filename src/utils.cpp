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



#include "config.hpp"
#include "utils.hpp"
#include "server.hpp"

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

  std::vector<std::string> get_files_in_directory(const std::string &directory) {
    std::vector<std::string> files;
    DIR *dir = opendir(directory.c_str());
    if (!dir) {
      EXIT("Failed to open directory: ");
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr) {
      if (entry->d_type == DT_REG) { 
        files.push_back(entry->d_name);
      }
    }
    closedir(dir);
    return files;
  }

  std::string read_file(const std::string &filepath) {
    std::ifstream file(filepath, std::ios::in | std::ios::binary);
    
    if(!file) {
      ERROR("Failed to open file: " + filepath);
    }

    std::ostringstream oss;
    oss << file.rdbuf();
    return oss.str();
  }

  std::string get_path(const std::string &filename) { 
    std::string ext = filename.substr(filename.find_last_of('.'));
    if(filename == "index.html") return "/";
    else if (ext == ".html") return "/" + filename.substr(0, filename.find_last_of('.'));
    else return "/" + filename;
  }

  std::string get_file_mimetype(const std::string &filename) {
    std::string extension = filename.substr(filename.find_last_of('.') + 1);
    if(extension == "html") return "text/html";
    else if(extension == "css") return "text/css";
    else if(extension == "js") return "application/javascript";
    return "text/html"; 
  }
}