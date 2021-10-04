cmake_minimum_required(VERSION 3.19)
project(str)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(str
        include/tiny/str.h
        include/tiny/re.h
        include/tiny/escape.h
        include/tiny/unescape.h
        src/str.cpp
        src/re.cpp
        src/escape.cpp
        src/unescape.cpp
        test/main.cpp)
