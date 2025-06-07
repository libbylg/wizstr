include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        str.cmake
        VERSION ${PACKAGE_VERSION}
        COMPATIBILITY AnyNewerVersion
        )