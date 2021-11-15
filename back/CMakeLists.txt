cmake_minimum_required(VERSION 3.20)
project(project_tp)

set(CMAKE_CXX_STANDARD 14)

add_library(player STATIC project/src/player_class.cpp)
add_library(map STATIC project/src/map_class.cpp)
add_library(game STATIC project/src/game_class.cpp project/include/objects.h project/include/player.h)
target_include_directories(player PUBLIC project/include)
target_include_directories(map PUBLIC project/include)
target_include_directories(game PUBLIC project/include)

add_executable(project_tp project/src/main.cpp)
target_link_libraries(project_tp game player map)

