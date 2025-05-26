// /example/main.cpp

#include "server.hpp"

using namespace http_server;

int main() {

  server_configuration config;
  config.port = 8080;
  config.log_file_path = "/mnt/d/KARTHIKEYA/PROJECTS/YO2/http_server/example/server.log";

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
      {"name", "Karthikeya(me)"},
      {"city", "Hyd, Telangana, India"},
      {"age", "20"},
      {"hobbies", "[\"coding\", \"reading\"]"},
      {"is_student", "true"},
      {"skills", "[\"C++\", \"Python\", \"TS\"]"}
    });
  });

  server.post("/api/submit", [](request_t req, response_t &res) {
    auto data = req.body;
    res.status(200);
    res.message(data);
  });

  server.get("/re", [](request_t req, response_t &res) {
    res.redirect("/home");
  });

  server.serve_static("./public");

  server.start();

  return 0;
}