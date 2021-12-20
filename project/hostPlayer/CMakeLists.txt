
add_library(player STATIC src/player_class.cpp)
add_library(map STATIC src/map_class.cpp)
add_library(game STATIC src/game_class.cpp include/factory.h include/healingPotion.h)

add_subdirectory(server)
include_directories(server)
add_executable(project_tp src/main.cpp)
target_link_libraries(project_tp game player map server lib sfml-graphics sfml-window sfml-system sfml-audio ${Boost_LIBRARIES} -lpthread)

target_include_directories(player PUBLIC include)
target_include_directories(map PUBLIC include)
target_include_directories(game PUBLIC include)
