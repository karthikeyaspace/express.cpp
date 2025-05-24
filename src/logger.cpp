// /src/logger.cpp

#include <fstream>
#include <iostream>
#include <sstream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>

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

namespace http_server{

  static std::queue<std::string> log_queue;
  static std::mutex log_mutex;
  static std::condition_variable log_cv; // for thread signaling
  static std::thread log_thread;
  static std::string log_file;
  static std::atomic<bool> logger_running(false);

  void log_worker() {
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

  void start_log_thread(const std::string &log_file_path) {
    log_file = log_file_path;
    logger_running = true;
    log_thread = std::thread(log_worker);
  }

  void stop_log_thread() {
    logger_running = false;
    log_cv.notify_all();
    if(log_thread.joinable()) {
      log_thread.join();
    }
  }

  void log(const std::string &log_level, const std::string &log_message) {
    if(log_file.empty()) return;
    
    std::ostringstream oss;
    oss << "[" << log_level << "]" << get_time() << log_message;

    {
      std::lock_guard<std::mutex> lock(log_mutex);
      log_queue.push(oss.str());
    }

    log_cv.notify_all();
  }
}