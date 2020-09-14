cmake_minimum_required(VERSION 3.15)
project(graph)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(3dparty/args)
include_directories(3dparty/args/include)

add_executable(generator 01/src/generator/main.cpp 01/src/generator/generator.hpp 01/src/generator/generator.cpp 01/src/generator/configurator.hpp)
add_executable(monothread 01/src/monothread/main.cpp 01/src/monothread/graph.hpp 01/src/monothread/graph.cpp 01/src/monothread/configurator.hpp)
