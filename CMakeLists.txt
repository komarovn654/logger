cmake_minimum_required(VERSION 3.22.1)
project(LOGGER 
    VERSION 1.0.0
    DESCRIPTION "Logger for C/C++ projects"
    LANGUAGES C CXX)

include(GNUInstallDirs)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(LOGGER_BUILD_TESTS "Build logger tests" ${PROJECT_IS_TOP_LEVEL})
option(LOGGER_BUILD_EXAMPLES "Build logger examples" ${PROJECT_IS_TOP_LEVEL})
option(LOGGER_INSTALL "Generate target for installing mylib" ON)

set(LOGGER_LIBRARY_TYPE "${LOGGER_LIBRARY_TYPE}" CACHE STRING
    "Library type override for Logger (SHARED, STATIC, OBJECT, or empty to follow BUILD_SHARED_LIBS)")

if (LOGGER_LIBRARY_TYPE)
    if (LOGGER_LIBRARY_TYPE STREQUAL "SHARED")
        set(LOGGER_BUILD_SHARED_LIBRARY TRUE)
    else()
        set(LOGGER_BUILD_SHARED_LIBRARY FALSE)
    endif()
else()
    set(LOGGER_BUILD_SHARED_LIBRARY ${BUILD_SHARED_LIBS})
endif()

message(SOURCE_DIR: ${LOGGER_SOURCE_DIR})
message(VERSION: "${LOGGER_VERSION_MAJOR}.${LOGGER_VERSION_MINOR}")
message(INSTALL: "${CMAKE_INSTALL_PREFIX}")
message(TYPE: "${LOGGER_LIBRARY_TYPE}")
message(IS_TOP: "${PROJECT_IS_TOP_LEVEL}")

#--------------------------------------------------------------------
# Add subdirectories
#--------------------------------------------------------------------
add_subdirectory(src)

# if (LOGGER_BUILD_TESTS)
#     add_subdirectory(tests)
# endif()

# if (LOGGER_BUILD_EXAMPLES)
#     add_subdirectory(examples)
# endif()
