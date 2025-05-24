// /src/server.cpp

#include "server.hpp"
#include "config.hpp"
#include "utils.hpp"

namespace http_server {

  HttpServer::HttpServer(const server_configuration &config)
    : config(config) {
  }


  HttpServer::~HttpServer() {
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

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
    server_addr.sin_port = htons(config.port);

    if (bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
      EXIT("Failed to bind socket");
    }

    if (listen(server_fd, config.max_connections) == -1) {
      EXIT("Failed to listen on socket");
    }

    std::cout << "Server is running on http://localhost:" << config.port << std::endl;

    this->config.server_fd = server_fd;
  }

  void HttpServer::acceptConnections() {
    while (true) {
      struct sockaddr_in client_addr{};
      socklen_t client_addr_len = sizeof(client_addr);

      int client_fd = accept(config.server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
      if (client_fd == -1) {
        perror("Failed to accept connection");
        continue;
      }


      handleClient(client_fd);
    }
  }

  void HttpServer::handleClient(int client_fd) {
    char buffer[BUFFER_SIZE] = {0};
    ssize_t bytes_received = read(client_fd, buffer, BUFFER_SIZE);
    

    if (bytes_received <= 0) {
      close(client_fd);
      return;
    }

    request_t req = parse(std::string(buffer));
    std::cout << "Received request: " << req.path << std::endl;

    response_t res;

    const auto& method = req.method;
    const auto& path = req.path;

    if(routes.count(method) && routes[method].count(path)) {
      auto handler = routes[method][path];
      std::cout << "handle client invoked" << std::endl;

      handler(req, res);
    } else {
      res.status(404);
    }

    std::string res_string = res.prepare_response();
    std::cout << res_string << std::endl;
    write(client_fd, res_string.c_str(), res_string.size());
    close(client_fd);
  }


  void HttpServer::start() {
    setupServer();
    acceptConnections();
  }

} // namespace http_server
