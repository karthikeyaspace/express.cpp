// /include/logger.hpp

#pragma once

// TODO: class Logger

namespace express {
  class Logger{
  public:
    Logger(const std::string &log_file_path);  

    ~Logger(){
      stop_log_thread();
    }

    static std::queue<std::string> log_queue;
    static std::string log_file;

    static std::mutex log_mutex;
    static std::condition_variable log_cv; // for thread signaling
    static std::thread log_thread;
    static std::atomic<bool> logger_running;

    void start_log_thread();
    void log(const std::string &log_level, const std::string &log_message);
    void stop_log_thread();

  private:
    void log_worker();
  };

} // namespace express