cmake_minimum_required(VERSION 3.19)
project(str)

set(CMAKE_CXX_STANDARD 11)

find_package(PCRE2 REQUIRED COMPONENTS "8BIT")
message("* PCRE2 [ ${PCRE2_INCLUDE_DIR} - ${PCRE2_LIBRARIES} ]")

find_package(GTest REQUIRED COMPONENTS)
message("* GTest [ ${GTEST_INCLUDE_DIRS} - ${GTEST_LIBRARIES} ]")

#find_package(Catch2 REQUIRED)
#message("* [ ${CATCH2_INCLUDE_DIRS} - ${CATCH2_LIBRARIES} ]")


include(test/test.cmake)
include(src/src.cmake)
#enable_testing()

set(CMAKE_VERBOSE_MAKEFILE ON)


add_executable(str ${SOURCES})


target_include_directories(str PRIVATE include ${INCLUDEPATH} ${PCRE2_INCLUDE_DIRS} ${GTEST_INCLUDE_DIR})
target_link_libraries(str PRIVATE ${PCRE2_LIBRARIES}  ${GTEST_LIBRARIES} GTest::gtest GTest::gtest_main)

#target_include_directories(str PRIVATE include ${INCLUDEPATH} ${PCRE2_INCLUDE_DIRS} ${CACHE2_INCLUDE_DIRS})
