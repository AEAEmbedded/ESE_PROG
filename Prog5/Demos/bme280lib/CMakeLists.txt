cmake_minimum_required(VERSION 3.16)
project(bme280 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BME280_BUILD_TESTS "Build the host unit tests (doctest via FetchContent)" ON)

# --- core: no platform code, builds everywhere -------------------------------
add_library(bme280 STATIC src/Bme280.cpp)
target_include_directories(bme280 PUBLIC src)
target_compile_options(bme280 PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wshadow)

# --- Linux platform layer + example ------------------------------------------
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_library(bme280_linux STATIC src/LinuxI2cBus.cpp)
    target_link_libraries(bme280_linux PUBLIC bme280)
    target_compile_options(bme280_linux PRIVATE -Wall -Wextra -Wpedantic)

    find_package(Threads REQUIRED)
    add_executable(bme280_rpi examples/rpi/main.cpp)
    target_link_libraries(bme280_rpi PRIVATE bme280_linux Threads::Threads)
endif()

# --- host tests --------------------------------------------------------------
if(BME280_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
