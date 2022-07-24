cmake_minimum_required(VERSION 3.22)
project(cjson5)

set(CMAKE_CXX_STANDARD 14)

add_compile_definitions(RUNNING_MAIN)

add_executable(cjson5 src/cjson5.cpp)
