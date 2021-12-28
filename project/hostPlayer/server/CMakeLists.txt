cmake_minimum_required(VERSION 3.16)
project(testingBoostAsio)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(../../include/)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
# Multithread Async HTTP Server
#add_library(lib ${LIB_HEADERS} ${LIB_SOURCES})
#target_include_directories(lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(server connection.cpp server.cpp connection.h server.h ../../include/message.h)
