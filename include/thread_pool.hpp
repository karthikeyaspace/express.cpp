// /include/thread.hpp

#pragma once

#include <thread>
#include <condition_variable>
#include <atomic>
#include <queue>
#include <mutex>
#include <functional>

#include "config.hpp"

// thread for logging, request queing (for multiple connections)

namespace http_server {
  
  class ThreadPool {
    public:
      ThreadPool(size_t num_threads);
      ~ThreadPool();

      static void request_worker(ThreadPool *pool);
      void enqueue(int client_fd);
      void stop();

      std::function<void(int)> client_handler;

    private:
      std::vector<std::thread> workers;
      std::queue<int> requests;
      std::mutex req_queue_mutex; 
      std::condition_variable req_cv;
      std::atomic<bool> pool_running;
    };
  
} // namespace http_server