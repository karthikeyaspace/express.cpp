// /example/main.cpp

#include "express.h"

using namespace express;

int main() {

  server_configuration config;
  config.port = 8080;
  config.log_file_path = "../example/server.log";

  HttpServer server(config);

  std::string static_path = "../example/public"; // path relative to /build/express.exe
  server.serve_static(static_path);

  server.get("/landing", [](Request req, Response &res) {
    res.status(200);
    res.message("Welcome to the landing page!");
  });

  server.get("/json", [](Request req, Response &res) {
    res.status(200);
    
  });

  server.post("/api/submit", [](Request req, Response &res) {
    auto data = req.body;
    res.status(200);
    res.message(data);
  });

  server.get("/api/data", [](Request req, Response &res) {
    res.status(200);
    res.json({
      {"message", "This is a sample API response"},
      {"status", "success"},
    });
  });

  server.get("/redirect", [](Request req, Response &res) {
    res.redirect("/");
  });

  server.start();

  return 0;
}