cmake_minimum_required(VERSION 3.14)
project(TemperatureConversionTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    CppUTest
    GIT_REPOSITORY https://github.com/cpputest/cpputest.git
    GIT_TAG master
)
set(TESTS OFF CACHE BOOL "")
FetchContent_MakeAvailable(CppUTest)

add_executable(test_temperature_conversion
    temperature_conversion.cpp
    test_temperature_conversion.cpp
    main.cpp
)

target_link_libraries(test_temperature_conversion PRIVATE CppUTest)
target_compile_options(test_temperature_conversion PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
