cmake_minimum_required(VERSION 3.19)
project(str)

set(CMAKE_CXX_STANDARD 14)

find_package(PCRE2 REQUIRED COMPONENTS "8BIT")

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

message("---- ${PCRE2_INCLUDE_DIR}")

target_include_directories(str PRIVATE include ${PCRE2_INCLUDE_DIR})
target_link_libraries(str PRIVATE ${PCRE2_LIBRARIES})
