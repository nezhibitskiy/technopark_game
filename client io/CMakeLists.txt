cmake_minimum_required(VERSION 3.20)
project(test)



find_package(SFML 2 COMPONENTS audio graphics window system network)

set(CMAKE_CXX_STANDARD 14)
set(SFML_INCLUDE_DIR "/home/rewieer/Libraries/SFML-2.5.1/include")
set(SFML_LIBRARY_DIR "/home/rewieer/Libraries/SFML-2.5.1/lib")
set(SFML_DIR "/home/rewieer/Libraries/SFML-2.5.1/lib/cmake/SFML")

#link_directories(${SFML_LIBRARY_DIR})
include_directories(${SFML_LIBRARY_DIR})

if(SFML_FOUND)
    message(STATUS "SFML_INCLUDE_DIR: ${SFML_INCLUDE_DIR}")
    message(STATUS "SFML_LIBRARIES: ${SFML_LIBRARIES}")
    message(STATUS "SFML_VERSION: ${SFML_VERSION}")
endif()



include_directories(include)
add_executable(test src/main.cpp src/State.cpp  src/Player.cpp src/StateStack.cpp
        src/Application.cpp src/MenuState.cpp src/GameState.cpp
        src/PreparationState.cpp src/Map.cpp src/Block.cpp src/Unit.cpp)


target_link_libraries (test sfml-graphics sfml-window sfml-system sfml-audio)