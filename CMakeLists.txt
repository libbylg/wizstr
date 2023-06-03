cmake_minimum_required(VERSION 3.16)
project(str)


#   Setup the global settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE ON)


#   Import external packages
#find_package(PCRE2 REQUIRED COMPONENTS "8BIT")
#find_package(GTest REQUIRED COMPONENTS)


#   Import sub-modules
include(test/test.cmake)
include(src/src.cmake)


#   Define the building target
add_executable(str_test ${SOURCES})
target_include_directories(str_test PRIVATE include ${INCLUDEPATH})
#target_link_libraries(str_test PRIVATE PCRE2::8BIT GTest::gtest GTest::gtest_main)
#target_compile_options(str_test PRIVATE -Wall -Wextra -DBYTES_USING_STL_CONTAINER)
target_compile_options(str_test PRIVATE -Wall -Wextra)
