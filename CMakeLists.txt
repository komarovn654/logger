# cmake_minimum_required(VERSION 3.22.1)
# project(logger 
#     VERSION 1.0.0
#     DESCRIPTION "Logger for C/C++ projects"
#     LANGUAGES C)

# set(sources logger/logger.c)
# set(public_headers logger/logger.h)

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

# message(BUILD_TESTING="${BUILD_TESTING}")

# if(BUILD_TESTING)
#     message(BUILD_TESTING="${BUILD_TESTING}")
#     message("TEST")
# endif()
cmake_minimum_required(VERSION 3.14)
project(my_project)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


enable_testing()

add_executable(
  hello_test
  tests/test.cpp
)

target_link_libraries(
  hello_test
  gtest_main
)

target_include_directories(hello_test PRIVATE /opt/homebrew/Cellar/googletest/1.14.0/include)
target_link_directories(hello_test PRIVATE /opt/homebrew/Cellar/googletest/1.14.0/lib)

include(GoogleTest)
gtest_discover_tests(hello_test)
