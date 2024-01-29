cmake_minimum_required(VERSION 3.22.1)
project(logger 
    VERSION 1.0.0
    DESCRIPTION "Logger for C/C++ projects"
    LANGUAGES C)

set(sources logger/logger.c)
set(public_headers logger/logger.h)

add_library(${PROJECT_NAME} SHARED ${sources})
set_target_properties(${PROJECT_NAME}  PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME}  PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
set_target_properties(${PROJECT_NAME}  PROPERTIES PUBLIC_HEADER ${public_headers})
set_target_properties(${PROJECT_NAME}  PROPERTIES INSTALL_NAME_DIR ${CMAKE_INSTALL_PREFIX}/lib)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME} 
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})