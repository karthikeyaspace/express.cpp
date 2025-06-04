<!-- /notes.md -->

HTTP server from scratch
(build like library)

Components:
1. TCP socket, binding to ports, listening to connections (base web server)
2. Request protocol parser - HTTP request lines and headers
3. API routing 
4. Handling headers
5. Multiple connections - thread pool (n threads) 
6. Serve static files
7. Logger
8. In-mem queue

Sources: 
1. https://developer.mozilla.org/en-US/docs/Web/HTTP


Managing
1. https://chatgpt.com/c/68309224-98e4-800e-ac1c-06643cf842f5
2. https://trello.com/c/YLy9GbYm/60-http-webserver-project


Commands explored:
- list all wsl distros -> wsl -l -v
- terminate wsl -> wsl --terminate <distro>
- set default wsl distro -> wsl --set-default <distro>
- install a distro -> wsl --install -d <distro>
- install packages in ubuntu -> sudo apt install build-essential cmake git -y
- set cmake compiler -> cmake .. -DCMAKE_C_COMPILER=/usr/bin/gcc -DCMAKE_CXX_COMPILER=/usr/bin/g++

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

Features:
- Serve static files
- Request logging 
- Parsing HTTP requests
- Multi-thread queueing
- API routing - GET, POST, PUT, DELETE
- Graceful shutdown
- Support for response codes
- Struct for server configuration - port, host, max_connections, rate_limit, cache_size, log_file, static files dir

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