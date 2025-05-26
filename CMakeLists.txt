cmake_minimum_required(VERSION 3.10)
project(HttpServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")


include_directories(include)

file(GLOB SOURCES "src/*.cpp" "example/*.cpp")

add_executable(http_server
    ${SOURCES}
)
