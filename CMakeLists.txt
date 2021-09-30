cmake_minimum_required(VERSION 3.19)
project(str)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(str
        include/str.h
        src/str.cpp
        test/main.cpp)
