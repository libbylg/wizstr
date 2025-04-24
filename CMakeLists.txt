cmake_minimum_required(VERSION 3.24)
project(str LANGUAGES CXX)

set(TEST_FRAMEWORK "gtest")
#set(TEST_FRAMEWORK "catch2")

#   Setup the global settings
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE ON)

list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_LIST_DIR}/depends)

if ("${TEST_FRAMEWORK}" STREQUAL "catch2")
    find_package(Catch2 3 REQUIRED)
else ()
    find_package(GTest REQUIRED)
endif ()


#   Import sub-modules
include(src/src.cmake)
include(test/test.cmake)

list(APPEND QUALITY_OPTIONS -Wall)
list(APPEND QUALITY_OPTIONS -Wextra)
list(APPEND QUALITY_OPTIONS -Werror=return-type)


#   Define the building target
add_executable(str_test ${SOURCES})
target_include_directories(str_test PRIVATE include ${INCLUDEPATH})
target_compile_options(str_test PRIVATE ${QUALITY_OPTIONS})
if ("${TEST_FRAMEWORK}" STREQUAL "catch2")
    target_link_libraries(str_test PRIVATE Catch2::Catch2WithMain Catch2::Catch2WithMain -static-libgcc -static-libstdc++)
else ()
    target_link_libraries(str_test PRIVATE GTest::gtest GTest::gtest_main)
endif ()

if ("${TEST_FRAMEWORK}" STREQUAL "catch2")
    target_compile_definitions(str_test PRIVATE TEST_FRAMEWORK=UT_WITH_CATCH2)
else ()
    target_compile_definitions(str_test PRIVATE TEST_FRAMEWORK=UT_WITH_GTEST)
endif ()


add_executable(gendoc doc/gendoc.cpp include/str.hpp src/str.cpp)
set_target_properties(gendoc PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/doc)
target_compile_options(gendoc PRIVATE ${QUALITY_OPTIONS})
target_include_directories(gendoc PRIVATE include ${INCLUDEPATH})
