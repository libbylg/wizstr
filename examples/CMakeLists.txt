# Copyright (c) 2021-2024 libbylg@126.com
# str is licensed under Mulan PSL v2.
# You can use this software according to the terms and conditions of the Mulan PSL v2.
# You may obtain a copy of Mulan PSL v2 at:
#          http://license.coscl.org.cn/MulanPSL2
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR
# FIT FOR A PARTICULAR PURPOSE.
# See the Mulan PSL v2 for more details.
cmake_minimum_required(VERSION 3.24)
project(examples)

#   Import quality configuration
include(${CMAKE_SOURCE_DIR}/quality.cmake)

#   Generate shared library
add_executable(examples example1.cpp ${CMAKE_SOURCE_DIR}/src/str.cpp ${CMAKE_SOURCE_DIR}/include/str.hpp)
target_include_directories(examples PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src)
target_compile_options(examples PRIVATE ${QUALITY_OPTIONS})
