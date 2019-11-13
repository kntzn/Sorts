cmake_minimum_required(VERSION 3.15)
project(Sorts)

set(CMAKE_CXX_STANDARD 14)

add_executable(Sorts main.cpp)

SET(CMAKE_CXX_FLAGS -pthread)