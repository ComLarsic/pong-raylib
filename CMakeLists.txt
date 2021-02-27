cmake_minimum_required(VERSION 3.17)
project(Pong)

set(CMAKE_CXX_STANDARD 14)

add_executable(Pong main.cpp)
target_link_libraries(Pong raylib)