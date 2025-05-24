HTTP server from scratch
(build like package)


Components:
1. TCP socket, binding to ports, listening to connections(base web server)
2. Request protocol parser - http request lines and headers
3. API routing 
4. Handing headers
5. Multiple connections - thread pool(n threads) 
6. Serve static files
7. rate limiting, caching
8. Logger
9. In-mem queue


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

Features
- Serve static files
- In-built rate limiting
- Request logging 
- Parsing http requests
- In-build in memory cache using LRU
- Multi thread queueing
- API routing - GET, POST, PUT, DELETE
- Graceful shutdown
- Support for response codes
- struct for server configuration - port, host, max_connections, rate_limit, cache_size, log_file, static files dir



Thread pool
- There are n threads, and 1 shared thread safe queue
- each thread will take a request from the queue and process it
- when a conn comes in, main acceptor thread will add it to the queue
- if queue is full, server may reject the request(503) or buffer it in bounded common queue - currently just reject
