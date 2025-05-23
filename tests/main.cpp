// /tests/main.cpp

#include "server.hpp"

using namespace http_server;

int main() {

  server_configuration config;
  config.port = 42069;
   
  HttpServer server(config);

  server.get("/", [](request_t req) -> response_t {
    response_t res;
    res.status_code = 200;
    res.status_message = "OK";
    res.body = "Hola";
    res.headers["Content-Type"] = "text/plain";
    return res;
  });

  server.get("/home", [](request_t req) -> response_t {
    response_t res;
    res.status_code = 200;
    res.status_message = "OK";
    res.body = "Welcome to the home page!";
    res.headers["Content-Type"] = "text/plain";
    return res;
  });

  server.start();

  return 0;
}