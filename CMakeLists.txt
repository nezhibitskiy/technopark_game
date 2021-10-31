cmake_minimum_required(VERSION 3.16)
project(chainOfResponsability)

set(CMAKE_CXX_STANDARD 17)

include_directories(move)
include_directories(handler)
include_directories(message)
add_executable(chainOfResponsability main.cpp)