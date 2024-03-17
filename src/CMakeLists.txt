add_library(logman ${LOGMAN_LIBRARY_TYPE}
                 "${PROJECT_SOURCE_DIR}/include/logman/logman.h"
                 logman_int.h logman.c)
# add_library(logman::logman ALIAS logman)

set_target_properties(logman PROPERTIES 
                            OUTPUT_NAME logman
                            VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
                            SOVERSION ${PROJECT_VERSION_MAJOR}
                            INSTALL_NAME_DIR ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}
                            DEFINE_SYMBOL _LOGMAN_BUILD_DLL
                            FOLDER "logman")

target_include_directories(logman PUBLIC
                           "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
target_include_directories(logman PRIVATE
                           "${PROJECT_SOURCE_DIR}/src"
                           "${PROJECT_BINARY_DIR}/src")

if (LOGMAN_BUILD_SHARED_LIBRARY)
    if (WIN32)
        if (MINGW)
            # Remove the dependency on the shared version of libgcc
            # NOTE: MinGW-w64 has the correct default but MinGW needs this
            target_link_libraries(logman PRIVATE "-static-libgcc")

            # Remove the lib prefix on the DLL (but not the import library)
            set_target_properties(logman PROPERTIES PREFIX "")

            # Add a suffix to the import library to avoid naming conflicts
            set_target_properties(logman PROPERTIES IMPORT_SUFFIX "dll.a")
        else()
            # Add a suffix to the import library to avoid naming conflicts
            set_target_properties(logman PROPERTIES IMPORT_SUFFIX "dll.lib")
        endif()
        set (LOGMAN_LIB_NAME_SUFFIX "dll")

        target_compile_definitions(logman INTERFACE LOGMAN_DLL)
    endif()

    if (MINGW)
        # Enable link-time exploit mitigation features enabled by default on MSVC
        include(CheckCCompilerFlag)

        # Compatibility with data execution prevention (DEP)
        set(CMAKE_REQUIRED_FLAGS "-Wl,--nxcompat")
        check_c_compiler_flag("" _LOGMAN_HAS_DEP)
        if (_LOGMAN_HAS_DEP)
            target_link_libraries(logman PRIVATE "-Wl,--nxcompat")
        endif()

        # Compatibility with address space layout randomization (ASLR)
        set(CMAKE_REQUIRED_FLAGS "-Wl,--dynamicbase")
        check_c_compiler_flag("" _LOGMAN_HAS_ASLR)
        if (_LOGMAN_HAS_ASLR)
            target_link_libraries(logman PRIVATE "-Wl,--dynamicbase")
        endif()

        # Compatibility with 64-bit address space layout randomization (ASLR)
        set(CMAKE_REQUIRED_FLAGS "-Wl,--high-entropy-va")
        check_c_compiler_flag("" _LOGMAN_HAS_64ASLR)
        if (_LOGMAN_HAS_64ASLR)
            target_link_libraries(logman PRIVATE "-Wl,--high-entropy-va")
        endif()

        # Clear flags again to avoid breaking later tests
        set(CMAKE_REQUIRED_FLAGS)
    endif()

    if (UNIX)
        # Hide symbols not explicitly tagged for export from the shared library
        target_compile_options(logman PRIVATE "-fvisibility=hidden")
    endif()
endif()

if (LOGMAN_INSTALL AND NOT CMAKE_SKIP_INSTALL_RULES)
    install(TARGETS logman
            EXPORT logmanTargets
            RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
            ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
endif()
