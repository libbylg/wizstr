# Copyright (c) 2021-2025  libbylg@126.com
# str is licensed under Mulan PSL v2.
# You can use this software according to the terms and conditions of the Mulan PSL v2.
# You may obtain a copy of Mulan PSL v2 at:
#          http://license.coscl.org.cn/MulanPSL2
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR
# FIT FOR A PARTICULAR PURPOSE.
# See the Mulan PSL v2 for more details.
cmake_minimum_required(VERSION 3.24)
project(gendoc LANGUAGES CXX)

#   Import quality configuration
include(${CMAKE_SOURCE_DIR}/cmake/quality.cmake)

#   Add major source
list(APPEND INCPATH ${CMAKE_SOURCE_DIR}/include)
list(APPEND SOURCES ${CMAKE_SOURCE_DIR}/include/str.hpp)

list(APPEND INCPATH ${CMAKE_SOURCE_DIR}/src)
list(APPEND SOURCES ${CMAKE_SOURCE_DIR}/src/str.cpp)

list(APPEND INCPATH ${CMAKE_CURRENT_LIST_DIR}/src)
list(APPEND SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/gendoc.cpp)

list(APPEND INCPATH ${CMAKE_CURRENT_LIST_DIR}/include)
list(APPEND SOURCES ${CMAKE_CURRENT_LIST_DIR}/include/gendoc.h)

#   Document generation tools
add_executable(gendoc EXCLUDE_FROM_ALL ${SOURCES})
target_include_directories(gendoc PRIVATE ${INCPATH})
target_compile_options(gendoc PRIVATE ${QUALITY_OPTIONS})
set_target_properties(gendoc PROPERTIES EXCLUDE_FROM_ALL TRUE)
set_target_properties(gendoc PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/templates")
