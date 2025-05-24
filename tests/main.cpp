// /tests/main.cpp

#include "server.hpp"

using namespace http_server;

int main() {

  server_configuration config;
  config.port = 8080;
  config.log_file_path = "/mnt/d/KARTHIKEYA/PROJECTS/YO2/http_server/tests/server.log";
   
  HttpServer server(config);

  server.get("/", [](request_t req, response_t &res) {
    res.status(200);
    res.message("Hola");
  });

  server.get("/home", [](request_t req, response_t &res) {
    res.status(200);
    res.message("Welcome to the home page!");
  });

  server.get("/json", [](request_t req, response_t &res) {
    res.status(200);
    res.json({
      {"key", "value"},
      {"key2", "value2"}
    });
  });

  server.start();

  return 0;
}