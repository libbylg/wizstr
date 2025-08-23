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

#   Read project version from local file
file(READ "${CMAKE_SOURCE_DIR}/VERSION" PROJECT_VERSION_FILE)
set(PROJECT_VERSION "${PROJECT_VERSION_FILE}")
string(STRIP "${PROJECT_VERSION}" PROJECT_VERSION)

#   Definition of the top project
project(wizstr VERSION ${PROJECT_VERSION})

#   Support custom the namespace
set(WIZSTR_NAMESPACE "" CACHE STRING "Used to given a namespace for this library, The default is empty")
if (WIZSTR_NAMESPACE STREQUAL "")
    file(WRITE "${CMAKE_SOURCE_DIR}/include/str_config.hpp"
            "#ifndef WIZSTR_CONFIG_H\n"
            "#define WIZSTR_CONFIG_H\n"
            "#endif\n"
    )
else ()
    file(WRITE "${CMAKE_SOURCE_DIR}/include/str_config.hpp"
            "#ifndef WIZSTR_CONFIG_H\n"
            "#define WIZSTR_CONFIG_H\n"
            "#define WIZSTR_NAMESPACE ${WIZSTR_NAMESPACE}\n"
            "#endif\n"
    )
endif ()

#   Import quality configuration
include(${CMAKE_SOURCE_DIR}/cmake/quality.cmake)

#   Generate shared library
add_library(wizstr-shared SHARED src/str.cpp include/str.hpp)
target_include_directories(wizstr-shared PRIVATE include src)
target_compile_options(wizstr-shared PRIVATE ${QUALITY_OPTIONS})
set_target_properties(wizstr-shared PROPERTIES OUTPUT_NAME "wizstr")
set_target_properties(wizstr-shared PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION})

#   Generate static library
add_library(wizstr-static STATIC src/str.cpp include/str.hpp)
target_include_directories(wizstr-static PRIVATE include src)
target_compile_options(wizstr-static PRIVATE ${QUALITY_OPTIONS})
set_target_properties(wizstr-static PROPERTIES OUTPUT_NAME "wizstr")

#   Unit-testing
add_subdirectory(tests)

#   All templates
add_subdirectory(templates)

#   All tools
add_subdirectory(tools)

#   Install all libs
install(TARGETS wizstr-shared wizstr-static
        EXPORT wizstr
        LIBRARY DESTINATION lib         # Shared library install target directory
        ARCHIVE DESTINATION lib         # Static library install target directory
)

install(FILES include/str.hpp DESTINATION include)
install(FILES include/str_config.hpp DESTINATION include)

install(EXPORT wizstr
        FILE wizstrTargets.cmake
        NAMESPACE wizstr::
        DESTINATION lib/cmake/wizstr
)


#   Generate Config for find_package
include(CMakePackageConfigHelpers)

configure_package_config_file(
        "${CMAKE_SOURCE_DIR}/cmake/config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/wizstrConfig.cmake"
        INSTALL_DESTINATION lib/cmake/wizstr
)

write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/wizstrConfigVersion.cmake"
        VERSION "${PROJECT_VERSION}"
        COMPATIBILITY AnyNewerVersion
)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/wizstrConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/wizstrConfigVersion.cmake"
        DESTINATION lib/cmake/wizstr
)
