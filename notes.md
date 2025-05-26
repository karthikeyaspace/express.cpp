HTTP server from scratch
(build like package)

Components:
1. TCP socket, binding to ports, listening to connections (base web server)
2. Request protocol parser - HTTP request lines and headers
3. API routing 
4. Handling headers
5. Multiple connections - thread pool (n threads) 
6. Serve static files
7. Rate limiting
8. Logger
9. In-mem queue
10. Middleware 

Sources: 
1. https://osasazamegbe.medium.com/showing-building-an-http-server-from-scratch-in-c-2da7c0db6cb7
2. https://developer.mozilla.org/en-US/docs/Web/HTTP

list all wsl distros -> wsl -l -v
terminate wsl -> wsl --terminate <distro>
set default wsl distro -> wsl --set-default <distro>
install a distro -> wsl --install -d <distro>
install packages in ubuntu -> sudo apt install build-essential cmake git -y
set cmake compiler -> cmake .. -DCMAKE_C_COMPILER=/usr/bin/gcc -DCMAKE_CXX_COMPILER=/usr/bin/g++

to build the project
- cd build
- cmake ..
- make
- ./http_server
- to build using g++ -> g++ -Iinclude src/*.cpp example/main.cpp -o http_server

Features
- Serve static files
- In-built rate limiting
- Request logging 
- Parsing HTTP requests
- In-built in-memory cache using LRU
- Multi-thread queueing
- API routing - GET, POST, PUT, DELETE
- Graceful shutdown
- Support for response codes
- Struct for server configuration - port, host, max_connections, rate_limit, cache_size, log_file, static files dir

Thread pool intro
- There are n threads, and 1 shared thread-safe queue
- Each thread will take a request from the queue and process it
- When a connection comes in, main acceptor thread will add it to the queue
- If queue is full, server may reject the request (503) or buffer it in bounded common queue - currently just reject

Done
- TCP socket, binding to ports, listening to connections (base web server)
- Request protocol parser - HTTP request lines and headers
- API routing (GET)
- Logger with background thread and in-memory queue to server.log file
- API routing (POST, PUT, DELETE)

TODO:
- Handling headers
- Serve static files
- In-memory queue for requests
- Multiple connections - Thread pool (n threads)
- Middleware support
- Rate limiting
- Load server config from yaml file
- terminal commands - r(restart), q(quit), p(show all paths)
- server.cors
