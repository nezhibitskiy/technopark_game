cmake_minimum_required(VERSION 3.20)
project(socket2)

set(CMAKE_CXX_STANDARD 14)

include_directories(client)
include_directories(server)

add_executable(socket2
        client/Clien.h
        client/main_client.cpp
        server/main.cpp
        server/Serv.cpp
        server/Serv.h
        server/tests_server.cpp)

find_package(GTest REQUIRED)
set(SOURCES server/tests_server.cpp)
add_executable(runTests ${SOURCES})
target_link_libraries(runTests GTest::gtest utils)