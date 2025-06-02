// /include/logger.hpp

#pragma once

#include <thread>
#include <condition_variable>
#include <atomic>
#include <queue>
#include <mutex>

namespace express {
  class Logger{
  public:
    Logger(const std::string &log_file_path);  

    ~Logger(){
      stop_log_thread();
    }

    std::queue<std::string> log_queue;
    std::string log_file;

    std::mutex log_mutex;
    std::condition_variable log_cv; // for thread signaling
    std::thread log_thread;
    std::atomic<bool> logger_running;

    void start_log_thread();
    void log(const std::string &log_level, const std::string &log_message);
    void stop_log_thread();

  private:
    void log_worker();
  };

} // namespace express