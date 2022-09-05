cmake_minimum_required(VERSION 3.21)
project(pair_algo)

set(CMAKE_CXX_STANDARD 20)
find_package(benchmark REQUIRED)
include_directories(.)

add_executable(pair_algo
        Exception.h
        Exception.cpp
        Tick.h
        Tick.cpp
        Asset.h
        Asset.cpp
        main.cpp)

target_link_libraries(pair_algo benchmark::benchmark)

add_executable(pair_algo_original
        Exception.h
        Exception.cpp
        Tick.h
        Tick.cpp
        Asset.h
        Asset.cpp
        main_originial.cpp)