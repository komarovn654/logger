add_library(logman ${LOGMAN_LIBRARY_TYPE}
                 "${PROJECT_SOURCE_DIR}/include/logman/logman.h"
                 logman_int.h logman.c)

set_target_properties(logman PROPERTIES 
                            VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
                            SOVERSION ${PROJECT_VERSION_MAJOR})
                            # INSTALL_NAME_DIR ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})

if (LOGMAN_INSTALL)
    install(TARGETS logman
            RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
            ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")
endif()
