cmake_minimum_required(VERSION 3.16)
project(str LANGUAGES CXX)


#   Setup the global settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_LIST_DIR}/depends)

find_package(Catch2 3 REQUIRED)
#find_package(GTest REQUIRED)

#   Import external packages
#find_package(PCRE2 REQUIRED COMPONENTS "8BIT")
#find_package(GTest REQUIRED COMPONENTS)


#   Import sub-modules
include(test/test.cmake)
include(src/src.cmake)


#   Define the building target
add_executable(str_test ${SOURCES})
target_include_directories(str_test PRIVATE include ${INCLUDEPATH})
#target_compile_options(str_test PRIVATE -static-libgcc -static-libstdc++)
target_link_libraries(str_test PRIVATE Catch2::Catch2WithMain Catch2::Catch2WithMain -static-libgcc -static-libstdc++)

#target_link_libraries(str_test PRIVATE GTest::gtest GTest::gtest_main)

#target_link_libraries(str_test PRIVATE PCRE2::8BIT GTest::gtest GTest::gtest_main)
#target_compile_options(str_test PRIVATE -Wall -Wextra -I${CMAKE_CURRENT_LIST_DIR}/catch2/include)
#target_compile_options(str_test PRIVATE -Wall -Wextra)
#target_link_options(str_test PRIVATE -L${CMAKE_CURRENT_LIST_DIR}/catch2/lib -lCatch2Main -lCatch2)
