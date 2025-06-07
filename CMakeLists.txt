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
project(str)

file(READ "${CMAKE_SOURCE_DIR}/VERSION" VERSION_CONTENT)
set(STR_VERSION ${VERSION_CONTENT})

#   Import quality configuration
include(${CMAKE_SOURCE_DIR}/quality.cmake)

#   Generate shared library
add_library(str-shared SHARED src/str.cpp include/str.hpp)
target_include_directories(str-shared PRIVATE include src)
target_compile_options(str-shared PRIVATE ${QUALITY_OPTIONS})
set_target_properties(str-shared PROPERTIES OUTPUT_NAME "str")
set_target_properties(str-shared PROPERTIES VERSION ${STR_VERSION} SOVERSION ${STR_VERSION})

#   Generate static library
add_library(str-static STATIC src/str.cpp include/str.hpp)
target_include_directories(str-static PRIVATE include src)
target_compile_options(str-static PRIVATE ${QUALITY_OPTIONS})
set_target_properties(str-static PROPERTIES OUTPUT_NAME "str")

install(TARGETS str-shared str-static
        EXPORT str
        LIBRARY DESTINATION lib  # Shared library install target directory
        ARCHIVE DESTINATION lib  # Static library install target directory
        INCLUDES DESTINATION include  # Headers
        NAMESPACE str
)

#   Unit-testing
add_subdirectory(tests)

#   Documentation tools
add_subdirectory(tools)

#   Examples
add_subdirectory(examples)