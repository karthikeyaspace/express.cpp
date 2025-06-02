// /src/server.cpp

#include "server.hpp"

namespace express {

  HttpServer::HttpServer(const server_configuration &config)
    : config(config) {
  }

  HttpServer::~HttpServer() {
    logger->stop_log_thread();
    thread_pool->stop();
    if (config.server_fd != -1) {
      close(config.server_fd);
    }
  }

  void HttpServer::setupServer() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
      EXIT("Failed to establish socket");
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
      EXIT("Failed to set socket options");
    }

    std::cout<<"Socket established"<<std::endl;
    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
    server_addr.sin_port = htons(config.port);
    
    if (bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
      EXIT("Failed to bind socket");
    }
    
    std::cout<<"Socket binded to port"<<std::endl;
    
    if (listen(server_fd, config.max_connections) == -1) {
      EXIT("Failed to listen on socket");
    }
    
    this->config.server_fd = server_fd;
    
    
    thread_pool = std::make_unique<ThreadPool>(config.threads);
    thread_pool->client_handler = [this](int client_fd) {
      this->handleClient(client_fd);
    };
    
    std::cout<<"Thread pool initialised" <<std::endl; 
    
    if(!config.log_file_path.empty()) {
      std::cout<<"Logger thread initialised "<<std::endl;
      logger = std::make_unique<Logger>(config.log_file_path);
      logger->start_log_thread();
    } else {
      WARNING("File logging disabled");
    }
    std::cout << "Server is running on http://localhost:" << config.port << std::endl;
  }


  void HttpServer::serve_static(const std::string &document_root) {
    config.document_root = document_root;
    auto files = get_files_in_directory(document_root);
    for (const auto &file: files) {
      std::string path = get_path(file);
      std::string full_path = document_root + "/" + file;
      this->get(path, [full_path](const Request req, Response &res) {
        std::string contents = read_file(full_path);
        std::string mimetype = get_file_mimetype(full_path);

        res.status(200);
        res.set_content_type(mimetype);
        res.body = contents;
      });
    }
  }

  void HttpServer::acceptConnections() {
    while (true) {
      struct sockaddr_in client_addr{};
      socklen_t client_addr_len = sizeof(client_addr);

      // new thread
      int client_fd = accept(config.server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
      if (client_fd == -1) {
        perror("Failed to accept connection");
        continue;
      }
   
      thread_pool->enqueue(client_fd);
    }

  }

  void HttpServer::handleClient(int client_fd) {
    char buffer[BUFFER_SIZE] = {0};
    ssize_t bytes_received = read(client_fd, buffer, BUFFER_SIZE);
    

    if (bytes_received <= 0) {
      close(client_fd);
      return;
    }

    // TODO: get client ip for rate limiting, logging
    std::string client_ip = get_client_ip(client_fd);

    Request req = parse(std::string(buffer));
    Response res;

    // logging request to log file
    this->logger->log("INFO", "Request: " + client_ip + " " + req.method + " " + req.path + "\n" + std::string(buffer));


    const auto& method = req.method;
    const auto& path = req.path;

    if(routes.count(method) && routes[method].count(path)) {
      auto handler = routes[method][path];
      handler(req, res);
    } else {
      res.status(404);
    }

    std::string res_string = res.prepare_response();

    // loggint response to log file
    this->logger->log("INFO", "Response: " + res_string + "\n\n");
    
    write(client_fd, res_string.c_str(), res_string.size());
    close(client_fd);
  }

  void HttpServer::helper(const std::string &alpha) {
    // TODO
    if (alpha == "h") {
      std::cout << "Available commands:\n"
                << "  r - show server routing table\n"
                << "  l - reload server with configuration and static files\n"
                << "  c - show server config\n"
                << "  q - quit\n\n";
    } else if (alpha == "r") {
      std::cout << "\nRegistered routes:\n";
      for (const auto& method : routes) {
        for (const auto& route : method.second) {
          std::cout << method.first << " " << route.first << "\n";
        }
      }
      std::cout << std::endl;
    } else if (alpha == "s") {
      std::cout << "Server is running on http://localhost:" << config.port << std::endl << std::endl;
    } else if (alpha == "q") {
      std::cout << "Stopping server...\n";
      if (config.server_fd != -1) {
        close(config.server_fd);
      }
      exit(0);
    }
  }

  void HttpServer::start() {
    setupServer();
    
    std::thread([this]() {
      std::string ip;
      while(true) {
        std::getline(std::cin, ip);
        this->helper(ip);
      }
    }).detach();

    acceptConnections();
  }

} // namespace express
