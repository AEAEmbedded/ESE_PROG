cmake_minimum_required(VERSION 3.10)

# Set the project name
project(MyTestProject)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find the GoogleTest package
find_package(GTest REQUIRED)

# Include the GoogleTest include directories
include_directories(${GTEST_INCLUDE_DIRS})

# Create a test executable
add_executable(my_test main.cpp)

# Link GoogleTest to the test executable
target_link_libraries(my_test ${GTEST_BOTH_LIBRARIES} pthread)
