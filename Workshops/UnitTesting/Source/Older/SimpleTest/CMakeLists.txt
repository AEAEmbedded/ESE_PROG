cmake_minimum_required(VERSION 3.27)
project(SimpleTest)

set(CMAKE_CXX_STANDARD 20)

# Download and build Google Test
include(FetchContent)
FetchContent_Declare(
        gtest
        URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
FetchContent_MakeAvailable(gtest)

# Create a library containing TemperatureConvertor
add_library(TemperatureConvertorLib
        TemperatureConvertor.cpp
        TemperatureConvertor.hpp)

# Add the executable for your main application
add_executable(SimpleTest main.cpp)
target_link_libraries(SimpleTest TemperatureConvertorLib)

# Add the executable for your tests
add_executable(SimpleUnitTests temperature_conversion_test.cpp)
target_link_libraries(SimpleUnitTests gtest_main TemperatureConvertorLib)

# Optionally add a test target for easier running in CLion
add_test(NAME RunTests COMMAND SimpleUnitTests)