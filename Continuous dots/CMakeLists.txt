cmake_minimum_required(VERSION 3.10)
project(Continuous_dots)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(Continuous_dots main.cpp geometry.cpp table.cpp bots.cpp game.cpp)
