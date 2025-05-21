
#include "server.hpp"
#include "config.hpp"
#include "utils.hpp"

namespace http_server {
  HttpServer::HttpServer(int port) : port(port), server_fd(-1) {}

  HttpServer::~HttpServer() {
    if (server_fd != -1) {
      close(server_fd);
    }
  }

  void HttpServer::setupServer() {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
      perror("Failed to establish socket");
      exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
      perror("Failed to set socket options");
      exit(EXIT_FAILURE);
    }
    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
      perror("Failed to bind socket");
      exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) == -1) {
      perror("Failed to listen on socket");
      exit(EXIT_FAILURE);
    }

    std::cout << "Server is running on http://localhost:" << port << std::endl;

  }

  void HttpServer::acceptConnections() {
    while (true) {
      struct sockaddr_in client_addr{};
      socklen_t client_addr_len = sizeof(client_addr);

      int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
      if (client_fd == -1) {
        perror("Failed to accept connection");
        continue;
      }

      char buffer[BUFFER_SIZE] = {0};
      ssize_t bytes_received = read(client_fd, buffer, BUFFER_SIZE);
      request_t req = parse(buffer);
      std::cout << "Received request: " << req.method << " " << req.path << " " << req.version << " " << req.body << std::endl;
      if (bytes_received > 0) {
        const char* response = "HTTP/1.1 200 OK\r\n"
                              "Content-Type: text/plain\r\n"
                              "Content-Length: 13\r\n"
                              "\r\n"
                              "Hello, World!";  
        send(client_fd, response, strlen(response), 0);
      }

      close(client_fd);
    }
  }

  void HttpServer::start() {
    setupServer();
    acceptConnections();
  }
}