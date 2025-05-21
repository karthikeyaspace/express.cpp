
#include "server.hpp"
#include "config.hpp"

using namespace http_server;

int main() {
  HttpServer server(PORT);
  server.start();
  return 0;
}