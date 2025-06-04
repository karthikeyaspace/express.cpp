// /example/main.cpp

#include "express.h"
#include "handler.h"

using namespace express;

int main() {

  server_configuration config;
  int port = 8080;
  config.port = port;
  config.log_file_path = "../example/server.log";

  HttpServer server(config);

  std::string static_path = "../example/public"; // path relative to /build/express.exe
  server.serve(static_path);

  server.get("/landing", [](Request req, Response &res) {
    res.status(200);
    res.message("Welcome to the landing page!");
  });

  server.get("/json", get_json);

  server.post("/api/submit", api_submit);

  server.get("/api/data", api_data);

  server.get("/redirect", redirect);

  server.start([&]() {
    std::cout << "Server is running on http://localhost:" << port << std::endl;
  });

  return 0;
}