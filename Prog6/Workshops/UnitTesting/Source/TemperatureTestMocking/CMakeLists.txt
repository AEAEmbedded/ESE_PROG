cmake_minimum_required(VERSION 3.14)
project(TemperatureControllerTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_BUILD_TYPE STREQUAL "Coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")
endif()

include(FetchContent)
FetchContent_Declare(
    CppUTest
    GIT_REPOSITORY https://github.com/cpputest/cpputest.git
    GIT_TAG master
)
set(TESTS OFF CACHE BOOL "")
FetchContent_MakeAvailable(CppUTest)

add_executable(test_temperature_controller
    temperature_controller.cpp
    test_temperature_controller.cpp
    main.cpp
)

target_link_libraries(test_temperature_controller PRIVATE CppUTest)
target_compile_options(test_temperature_controller PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)