// /src/logger.cpp

#include <fstream>
#include <iostream>
#include <sstream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>

#include "logger.hpp"
#include "utils.hpp"

/**
 * Logger
 * 1. Log thread instead of server waiting for io
 * 2. Log levels - INFO, DEBUG, ERROR
 * 3. Data format - req and res as json format, Errors as strings
 *
 * log format for req and res
 * [LEVEL][TIMESTAMP] ip_addr method path
 * [req/res DATA]
 *
 * log format for general messages
 * [LEVEL][TIMESTAMP] message
 */

namespace express {

  Logger::Logger(const std::string &log_file_path) {
    log_file = log_file_path;
  }

  void Logger::log_worker() {
    while(logger_running || !log_queue.empty()) {
      std::unique_lock<std::mutex> lock(log_mutex);
      log_cv.wait(lock, [] { return !log_queue.empty() || !logger_running; });

      while (!log_queue.empty()) {
        std::string log_message = log_queue.front();
        log_queue.pop();

        std::ofstream log_file_stream(log_file, std::ios::app);
        if (log_file_stream.is_open()) {
          log_file_stream << log_message << std::endl;
          log_file_stream.close();
        } else {
          std::cerr << "Failed to open log file: " << log_file << std::endl;
        }

      }
    }
  }

  void Logger::start_log_thread() {
    logger_running = true;
    log_thread = std::thread(log_worker);
  }

  void Logger::stop_log_thread() {
    logger_running = false;
    log_cv.notify_all();
    if(log_thread.joinable()) {
      log_thread.join();
    }
  }

  void Logger::log(const std::string &log_level, const std::string &log_message) {
    if(log_file.empty()) return;
    
    std::ostringstream oss;
    oss << "[" << log_level << "]" << get_time() << log_message;

    {
      std::lock_guard<std::mutex> lock(log_mutex);
      log_queue.push(oss.str());
    }

    log_cv.notify_all();
  }
} // namespace express