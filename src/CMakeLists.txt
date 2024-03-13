message(TYPE ${LOGMAN_LIBRARY_TYPE})
add_library(logman ${LOGMAN_LIBRARY_TYPE}
                 "${PROJECT_SOURCE_DIR}/include/logman/logman.h"
                 logman_int.h logman.c)

if (UNIX AND GLFW_LOGMAN_SHARED_LIBRARY)
    set(LOGMAN_LIB_NAME logman_sl)
else()
    set(LOGMAN_LIB_NAME logman)
endif()
set(LOGMAN_LIB_NAME_SUFFIX "")

set_target_properties(logman PROPERTIES 
                            VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
                            SOVERSION ${PROJECT_VERSION_MAJOR}
                            FOLDER "logman")

if (LOGMAN_INSTALL)
    install(TARGETS logman
            RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
            ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")
endif()
