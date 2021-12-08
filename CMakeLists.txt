cmake_minimum_required(VERSION 3.14)
project(project_tp)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(graphics)
include_directories(graphics/include)

add_subdirectory(project)

