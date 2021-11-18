cmake_minimum_required(VERSION 3.20)
project(project_tp)

set(CMAKE_CXX_STANDARD 17)

find_package(GTest REQUIRED)

add_library(map STATIC project/src/map_class.cpp)
target_include_directories(map PUBLIC project/include)
target_link_libraries(map PUBLIC)

add_library(game STATIC project/src/game_class.cpp)
target_include_directories(game PUBLIC project/include)
target_link_libraries(game PUBLIC map)

add_executable(test_map project/tests/test_map.cpp)
target_link_libraries(test_map GTest::gtest map)

add_executable(project_tp project/src/main.cpp)
target_link_libraries(project_tp game player map)