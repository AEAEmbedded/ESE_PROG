cmake_minimum_required(VERSION 3.14)
project(FirstTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# Fetch CppUTest
include(FetchContent)
FetchContent_Declare(
    CppUTest
    GIT_REPOSITORY https://github.com/cpputest/cpputest.git
    GIT_TAG master
)

set(TESTS OFF CACHE BOOL "")
FetchContent_MakeAvailable(CppUTest)

# Single test executable
add_executable(first_test
    test_first.cpp
    main.cpp
)
target_link_libraries(first_test PRIVATE CppUTest)