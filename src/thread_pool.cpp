// /src/thread_pool.cpp

#include "thread_pool.hpp"

namespace express{
  
  ThreadPool::ThreadPool(size_t num_threads){
    pool_running = true;
    for(size_t i=0; i<num_threads; i++) {
      workers.emplace_back(&ThreadPool::request_worker, this);
    }
  }

  ThreadPool::~ThreadPool() {
    stop();
  }

  void ThreadPool::request_worker(ThreadPool *pool) {
    while(pool->pool_running || !pool->requests.empty()) {
      int client_fd;
      {
        std::unique_lock<std::mutex> lock(pool->req_queue_mutex); 
        pool->req_cv.wait(lock, [pool] {
          return !pool->requests.empty() || !pool->pool_running;
        });

        client_fd = pool->requests.front();
        pool->requests.pop();
      }
      pool->client_handler(client_fd);
    }
  }

  void ThreadPool::enqueue(int client_fd) {
    {
      std::lock_guard<std::mutex> lock(req_queue_mutex);
      requests.push(client_fd);
    }
    req_cv.notify_one();
  }

  void ThreadPool::stop() {
    pool_running = false;
    req_cv.notify_all();
    for(auto &worker : workers) {
      if(worker.joinable()) {
        worker.join();
      }
    }
  }


} // namespace express