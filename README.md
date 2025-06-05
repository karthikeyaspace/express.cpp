# HTTP Server Library in C++

Express.js-inspired HTTP server written in C++

Features:
- Serve static files
- API routing: GET, POST, PUT, DELETE
- Multithreaded request handling with a thread pool
- In-memory request queue
- HTTP request parsing (method, headers, query params)
- Thread-based logging system
- URL query parameter handling (/json?length=5)
- File logging (non-blocking with background thread)
- Command-line triggers: press r to show routes, c to show configuration, q to stop server.


Components:
1. TCP socket, binding to ports, listening to connections (base web server)
2. Request protocol parser - HTTP request lines, headers, query parameters
3. Server configuration struct
4. Thread pool, In-memory request queue
5. Logger

Sources: 
1. https://developer.mozilla.org/en-US/docs/Web/HTTP


to build the project:
- wsl
- cd <project_directory> [make sure that project dir name is same as namespace]
- mkdir build
- cd build
- cmake ..
- make
- ./express
- to build using g++ -> g++ -Iinclude src/*.cpp example/main.cpp -o express

Make sure to have:
- cmake
- wsl or linux(Ubuntu to be specific)
- g++


Concurrency and Synchronization:
- We need 4 things to perform any task parallely - thread(std::thread), mutex(std::mutex), condition variable(std::condition_variable), and a atomic variable(std::atomic<bool>).
- Thread is a lightweight process that can run concurrently with other threads.
- Mutex is a mutual exclusion lock that allows only one thread to access a shared resource at a time. protecting shared buffer.
- Condition variable is a synchronization primitive that allows threads to wait for a condition to be met before proceeding.
- Atomic variable is a variable that can be accessed by multiple threads without the need for a mutex, ensuring that the value is always consistent, threads will know when to stop processing without taking a mutex unnecessarily.
- flow - request comes in -> add to queue -> cv notifies worker threads -> avaliable worker threads pop from queue -> process request   -> send response    

Thread pool:
- There are n threads, and 1 shared thread-safe queue
- Each thread will take a request from the queue and process it
- When a connection comes in, main acceptor thread will add it to the queue
- If queue is full, server may reject the request (503) or buffer it in bounded common queue - currently just reject
- We dont want to spawn a new thread for every request
- We will have a listener thread in acceptConnections, pushes client_fd to request_queue
- MAX_THREADS no. of worker threads will take reqs from queue, pop client_fd and handleClient()
- unique_lock is used when you need to lock and unlock the mutex multiple times
- lock_guard is used when you need to lock the mutex only once and it will automatically unlock when it goes out of scope  
- condition_variable is used to notify the worker threads when there is a new request in the queue