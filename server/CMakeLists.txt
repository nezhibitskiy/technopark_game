cmake_minimum_required(VERSION 3.16)
project(testingBoostAsio)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
# Multithread Async HTTP Server
add_executable(server connection.h connection.cpp request.h response.h server.h server.cpp message.h) # router.h)
target_link_libraries(server  ${Boost_LIBRARIES})

add_executable(client client.cpp)
target_link_libraries(client  ${Boost_LIBRARIES})
