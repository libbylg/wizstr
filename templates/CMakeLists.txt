# Copyright (c) 2021-2025  libbylg@126.com
# wizstr is licensed under Mulan PSL v2.
# You can use this software according to the terms and conditions of the Mulan PSL v2.
# You may obtain a copy of Mulan PSL v2 at:
#          http://license.coscl.org.cn/MulanPSL2
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR
# FIT FOR A PARTICULAR PURPOSE.
# See the Mulan PSL v2 for more details.
cmake_minimum_required(VERSION 3.24)
project(templates LANGUAGES CXX)

#   Import quality configuration
include(${CMAKE_SOURCE_DIR}/cmake/quality.cmake)

#   Add major source
list(APPEND INCPATH ${CMAKE_SOURCE_DIR}/include)
list(APPEND SOURCES ${CMAKE_SOURCE_DIR}/include/str.hpp)
list(APPEND INCPATH ${CMAKE_SOURCE_DIR}/src)
list(APPEND SOURCES ${CMAKE_SOURCE_DIR}/src/str.cpp)

#   Example applications for README
add_executable(example1 EXCLUDE_FROM_ALL example1.cpp ${SOURCES})
target_include_directories(example1 PRIVATE ${INCPATH})
target_compile_options(example1 PRIVATE ${QUALITY_OPTIONS})
set_target_properties(example1 PROPERTIES EXCLUDE_FROM_ALL TRUE)
set_target_properties(example1 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/templates")

#   Add fake target for: make doc
add_custom_target(doc COMMAND /bin/sh "${CMAKE_SOURCE_DIR}/templates/gendoc.sh")
add_dependencies(doc gendoc example1)
