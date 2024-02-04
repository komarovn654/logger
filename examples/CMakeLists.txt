add_executable(example demo_funcs.c demo_main.c ${log_headers})

target_include_directories(example PRIVATE ${log_src_dir})
target_link_directories(example PRIVATE ${CMAKE_BINARY_DIR}/logger)
target_link_libraries(example logger_static)

# link_libraries()

# add_library(logger SHARED ${log_sources})
# add_library(logger_static STATIC ${log_sources})

# set_target_properties(logger logger_static PROPERTIES VERSION ${PROJECT_VERSION})
# set_target_properties(logger logger_static PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
# set_target_properties(logger logger_static PROPERTIES PUBLIC_HEADER ${log_headers})
# set_target_properties(logger logger_static PROPERTIES INSTALL_NAME_DIR ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})

# install(TARGETS logger logger_static
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
