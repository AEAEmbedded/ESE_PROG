cmake_minimum_required(VERSION 3.27)
project(TemperatureConverter)

add_library(ConvertApp main.cpp TemperatureConversion.cpp)

add_executable(TestDemo_UnitTest UnitTests/conversion_test.cpp)
target_link_libraries(TestDemo_UnitTest ConvertApp GTest::gtest_main)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)

FetchContent_MakeAvailable(googletest)