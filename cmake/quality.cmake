#   Setup the global settings
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE ON)

#   Compile options for error check
list(APPEND QUALITY_OPTIONS -Wall)
list(APPEND QUALITY_OPTIONS -Wextra)
list(APPEND QUALITY_OPTIONS -Werror=return-type)
list(APPEND QUALITY_OPTIONS -Werror=sign-compare)
#list(APPEND QUALITY_OPTIONS -Werror=conversion)
list(APPEND QUALITY_OPTIONS -Werror=return-type)
#list(APPEND QUALITY_OPTIONS -Wno-unused-parameter)
