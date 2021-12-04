cmake_minimum_required(VERSION 3.16)
project(project_tp)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lsfml-graphics -lsfml-window -lsfml-system ")




include_directories(../project/include)

set(SFML_STATIC_LIBRARIES TRUE)
if(SFML_STATIC_LIBRARIES)
    add_definitions(-DSFML_STATIC)
endif()
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


set(LIB_SOURCES
        src/State.cpp  src/InputPlayer.cpp src/StateStack.cpp
        src/Application.cpp src/MenuState.cpp src/GameState.cpp
        src/PreparationState.cpp src/Map.cpp src/Block.cpp src/Unit.cpp include/Heart.cpp include/Heart.h include/EventManager.cpp include/EventManager.h)

set(LIB_HEADERS
        include/State.h
        include/InputPlayer.h
        include/StateStack.h
        include/Application.h
        include/MenuState.h
        include/GameState.h
        include/PreparationState.h
        include/Map.h
        include/Block.h
        include/Unit.h include/DrawHandlers.cpp include/DrawHandlers.h include/Heart.cpp include/Heart.h include/Handler.h include/EventManager.cpp include/EventManager.h)

add_library(lib ${LIB_HEADERS} ${LIB_SOURCES})
target_include_directories(lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)



add_executable(test src/main.cpp )

target_link_libraries(test lib game player map)
target_link_libraries (test sfml-graphics sfml-window sfml-system sfml-audio)


#find_package(GTest REQUIRED)
#include_directories(include)
#set(SOURCES tests/run_tests.cpp tests/input_test.cpp tests/resouces_test.cpp)
#add_executable(runTests ${SOURCES})
#target_link_libraries(runTests GTest::gtest lib)
#target_link_libraries(runTests GTest::gtest sfml-graphics sfml-window sfml-system sfml-audio)

