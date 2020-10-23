cmake_minimum_required(VERSION 3.15)
project(graph)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(3dparty/args)
include_directories(3dparty/args/include)

find_package(Qt5 COMPONENTS Concurrent REQUIRED)

add_executable(generator
        01/src/generator/main.cpp
        01/src/generator/generator.hpp
        01/src/generator/generator.cpp
        01/src/generator/configurator.hpp)
add_executable(monothread
        01/src/monothread/main.cpp
        01/src/monothread/graph.hpp
        01/src/monothread/graph.cpp
        01/src/monothread/configurator.hpp
        01/src/monothread/bellman_ford.hpp
        01/src/monothread/dijkstra.hpp
        01/src/monothread/extra_paths.hpp
        01/src/monothread/dfs.hpp
        01/src/monothread/dfs.cpp
        01/src/generator/generator.hpp
        01/src/generator/generator.cpp)
add_executable(multithread
        02/multithread/main.cpp
        02/multithread/configurator.hpp
        02/multithread/multithread.hpp
        01/src/monothread/graph.hpp
        01/src/monothread/graph.cpp
        01/src/monothread/configurator.hpp
        01/src/monothread/bellman_ford.hpp
        01/src/monothread/dijkstra.hpp
        01/src/monothread/extra_paths.hpp
        01/src/monothread/dfs.hpp
        01/src/monothread/dfs.cpp
        01/src/generator/generator.hpp
        01/src/generator/generator.cpp)
add_executable(mapreduce
        03/src/main.cpp
        03/src/configurator.hpp
        01/src/monothread/graph.hpp
        01/src/monothread/graph.cpp
        01/src/monothread/bellman_ford.hpp
        01/src/monothread/dfs.hpp
        01/src/monothread/dfs.cpp
        01/src/generator/generator.hpp
        01/src/generator/generator.cpp)

target_link_libraries(multithread pthread)
target_link_libraries(mapreduce Qt5::Concurrent)
