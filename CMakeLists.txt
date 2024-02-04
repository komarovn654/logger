cmake_minimum_required(VERSION 3.22.1)
project(logger 
    VERSION 1.0.0
    DESCRIPTION "Logger for C/C++ projects"
    LANGUAGES C CXX)

set(log_sources ${CMAKE_SOURCE_DIR}/logger/logger.c)
set(log_headers ${CMAKE_SOURCE_DIR}/logger/logger.h)
set(log_src_dir ${CMAKE_SOURCE_DIR}/logger/)

add_subdirectory(logger)
add_subdirectory(tests)
add_subdirectory(examples)

# add_library(${PROJECT_NAME} SHARED ${sources})
# set_target_properties(${PROJECT_NAME}  PROPERTIES VERSION ${PROJECT_VERSION})
# set_target_properties(${PROJECT_NAME}  PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
# set_target_properties(${PROJECT_NAME}  PROPERTIES PUBLIC_HEADER ${public_headers})
# set_target_properties(${PROJECT_NAME}  PROPERTIES INSTALL_NAME_DIR ${CMAKE_INSTALL_PREFIX}/lib)

# include(GNUInstallDirs)
# install(TARGETS ${PROJECT_NAME} 
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# enable_testing()
# include(GoogleTest)



# if(BUILD_TESTING)
#     message(BUILD_TESTING="${BUILD_TESTING}")
#     message("TEST")
# endif()


