add_library(logger ${LOGGER_LIBRARY_TYPE}
                 "${LOGGER_SOURCE_DIR}/include/logger/logger.h"
                 logger.c)

set_target_properties(logger PROPERTIES 
                            VERSION ${LOGGER_VERSION_MAJOR}.${LOGGER_VERSION_MINOR}
                            SOVERSION ${LOGGER_VERSION_MAJOR})
                            # INSTALL_NAME_DIR ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})

if (LOGGER_INSTALL)
    install(TARGETS logger
            RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
            ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")
endif()
