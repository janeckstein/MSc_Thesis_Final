cmake_minimum_required(VERSION 3.21)
project(Benchmarks_chapter3)

set(CMAKE_CXX_STANDARD 20)
find_package(benchmark REQUIRED)
include_directories(.)

add_executable(Benchmarks_chapter3 crtp_main.cpp normal_class.hpp crtp_class.hpp)
add_executable(slowpath_main slowpath_main.cpp slowpath_h.h)
add_executable(type_engineering type_engineering.cpp)

target_link_libraries(Benchmarks_chapter3 benchmark::benchmark)
target_link_libraries(slowpath_main benchmark::benchmark)
target_link_libraries(type_engineering benchmark::benchmark)