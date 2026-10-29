cmake_minimum_required(VERSION 3.29)
project(Test)

set(CMAKE_CXX_STANDARD 23)

MESSAGE(STATUS "Using toolchain file ➜ ${CMAKE_TOOLCHAIN_FILE}")

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.18.0
)
FetchContent_MakeAvailable(googletest)

add_executable(Test
    "test_matchengine.cpp"
    "mock/mockkeygen.h"
    "mock/mockkeyboard.h"
)

target_link_libraries(Test PRIVATE
    GTest::gtest
    GTest::gtest_main  # Provides a default main() function so don't have to write one
    GTest::gmock       # The mocking framework
)

target_include_directories(Test PRIVATE "${CMAKE_SOURCE_DIR}/src")

include(GoogleTest)
gtest_discover_tests(Test)