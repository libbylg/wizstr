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

#   Read project version from local file
file(READ "${CMAKE_SOURCE_DIR}/VERSION" PROJECT_VERSION_FILE)
set(PROJECT_VERSION "${PROJECT_VERSION_FILE}")
string(STRIP "${PROJECT_VERSION}" PROJECT_VERSION)

#   Definition of the top project
project(str VERSION ${PROJECT_VERSION})

#   Import quality configuration
include(${CMAKE_SOURCE_DIR}/cmake/quality.cmake)

#   Generate shared library
add_library(str-shared SHARED src/str.cpp include/str.hpp)
target_include_directories(str-shared PRIVATE include src)
target_compile_options(str-shared PRIVATE ${QUALITY_OPTIONS})
set_target_properties(str-shared PROPERTIES OUTPUT_NAME "str")
set_target_properties(str-shared PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION})

#   Generate static library
add_library(str-static STATIC src/str.cpp include/str.hpp)
target_include_directories(str-static PRIVATE include src)
target_compile_options(str-static PRIVATE ${QUALITY_OPTIONS})
set_target_properties(str-static PROPERTIES OUTPUT_NAME "str")

#   Unit-testing
add_subdirectory(tests)

#   All templates
add_subdirectory(templates)

#   Install all libs
install(TARGETS str-shared str-static
        EXPORT str
        LIBRARY DESTINATION lib         # Shared library install target directory
        ARCHIVE DESTINATION lib         # Static library install target directory
)

install(FILES include/str.hpp DESTINATION include)

install(EXPORT str
        FILE strTargets.cmake
        NAMESPACE str::
        DESTINATION lib/cmake/str
)


#   Generate Config for find_package
include(CMakePackageConfigHelpers)

configure_package_config_file(
        "${CMAKE_SOURCE_DIR}/cmake/config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/strConfig.cmake"
        INSTALL_DESTINATION lib/cmake/str
)

write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/strConfigVersion.cmake"
        VERSION "${PROJECT_VERSION}"
        COMPATIBILITY AnyNewerVersion
)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/strConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/strConfigVersion.cmake"
        DESTINATION lib/cmake/str
)
