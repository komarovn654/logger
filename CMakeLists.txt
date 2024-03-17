cmake_minimum_required(VERSION 3.22.1)
project(logman 
    VERSION 1.0.0
    DESCRIPTION "Logger for C/C++ projects"
    LANGUAGES C CXX)

include(GNUInstallDirs)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(LOGMAN_BUILD_TESTS "Build logman tests" ${PROJECT_IS_TOP_LEVEL})
option(LOGMAN_BUILD_EXAMPLES "Build logman examples" ${PROJECT_IS_TOP_LEVEL})
option(LOGMAN_INSTALL "Generate target for installing logman" ON)

set(LOGMAN_LIBRARY_TYPE "${LOGMAN_LIBRARY_TYPE}" CACHE STRING
    "Library type override for logman (SHARED, STATIC, OBJECT, or empty to follow BUILD_SHARED_LIBS)")

if (LOGMAN_LIBRARY_TYPE)
    if (LOGMAN_LIBRARY_TYPE STREQUAL "SHARED")
        set(LOGMAN_BUILD_SHARED_LIBRARY TRUE)
    else()
        set(LOGMAN_BUILD_SHARED_LIBRARY FALSE)
    endif()
else()
    set(LOGMAN_BUILD_SHARED_LIBRARY ${BUILD_SHARED_LIBS})
endif()

set(LOGMAN_SOURCES ${PROJECT_SOURCE_DIR}/src/logman.c)

#--------------------------------------------------------------------
# Create generated files
#--------------------------------------------------------------------
include(CMakePackageConfigHelpers)

set(LOGMAN_CONFIG_PATH "${CMAKE_INSTALL_LIBDIR}/cmake/logman")

write_basic_package_version_file(src/logmanConfigVersion.cmake
                                    VERSION ${PROJECT_VERSION}
                                    COMPATIBILITY SameMajorVersion)

configure_package_config_file(CMake/logmanConfig.cmake
                                src/logmanConfig.cmake
                                INSTALL_DESTINATION "${LOGMAN_CONFIG_PATH}"
                                NO_CHECK_REQUIRED_COMPONENTS_MACRO)



#--------------------------------------------------------------------
# Add subdirectories
#--------------------------------------------------------------------
add_subdirectory(src)

if (LOGMAN_BUILD_TESTS)
    add_subdirectory(tests)
endif()

if (LOGMAN_BUILD_EXAMPLES)                    
    add_subdirectory(examples)
endif()

#--------------------------------------------------------------------
# Install files other than the library
# The library is installed by src/CMakeLists.txt
#--------------------------------------------------------------------
if (LOGMAN_INSTALL AND NOT CMAKE_SKIP_INSTALL_RULES)
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/logman/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            FILES_MATCHING PATTERN logman.h)

    export(EXPORT logmanTargets
            FILE "${CMAKE_CURRENT_BINARY_DIR}/logman/logmanTargets.cmake")

    install(FILES "${PROJECT_BINARY_DIR}/src/logmanConfig.cmake"
                  "${PROJECT_BINARY_DIR}/src/logmanConfigVersion.cmake"
            DESTINATION "${LOGMAN_CONFIG_PATH}")

    install(EXPORT logmanTargets 
            FILE logmanTargets.cmake
            NAMESPACE logman::
            EXPORT_LINK_INTERFACE_LIBRARIES
            DESTINATION "${LOGMAN_CONFIG_PATH}")
endif()
