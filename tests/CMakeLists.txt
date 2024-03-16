# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
include(GoogleTest)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(BUILD_GTEST ON CACHE BOOL "" FORCE)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
option(INSTALL_GTEST "Enable installation of googletest." OFF)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    logman_test
    logman_utest.cpp
    logman_mtest.cpp
    ${LOGMAN_SOURCES}
)

target_compile_definitions(logman_test PRIVATE UTEST_BUILD=1)

target_link_libraries(
    logman_test
    gtest_main
)

gtest_discover_tests(logman_test)
    