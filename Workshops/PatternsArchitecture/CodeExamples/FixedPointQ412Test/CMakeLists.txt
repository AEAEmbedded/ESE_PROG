cmake_minimum_required(VERSION 3.14)
project(FixedPointQ412Test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch CppUTest from GitHub
include(FetchContent)
FetchContent_Declare(
    CppUTest
    GIT_REPOSITORY https://github.com/cpputest/cpputest.git
    GIT_TAG master
)
set(TESTS OFF CACHE BOOL "")
FetchContent_MakeAvailable(CppUTest)

# Create the test executable
add_executable(test_fixed_point_q412
    test_fixed_point_q412.cpp
    test_cs_kompakt_data.cpp
    main.cpp
)

# Link CppUTest
target_link_libraries(test_fixed_point_q412 PRIVATE CppUTest)

# Enable strict compiler warnings
target_compile_options(test_fixed_point_q412 PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
