cmake_minimum_required(VERSION 3.16)
project(rpi_platform LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(FATAL_ERROR "rpi_platform needs Linux i2c-dev; build the core and the tests on other hosts instead")
endif()

# --- The core: the week-2 starter library, untouched --------------------------
# Point this at your own library once you have one.
set(SENSOR_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../week2/bme280_starter/bme280" CACHE PATH "Sensor library root")
add_subdirectory(${SENSOR_LIB_DIR} ${CMAKE_BINARY_DIR}/sensor_lib)

# --- Platform layer: the only target that includes <linux/i2c-dev.h> -----------
add_library(rpi_platform STATIC src/LinuxI2cBus.cpp)
target_include_directories(rpi_platform PUBLIC include)
target_link_libraries(rpi_platform PUBLIC bme280)     # for the Bus interface header
target_compile_options(rpi_platform PRIVATE -Wall -Wextra -Wpedantic)

# --- Example -----------------------------------------------------------------
add_executable(rpi_smoke_test examples/rpi_smoke_test.cpp)
target_link_libraries(rpi_smoke_test PRIVATE rpi_platform)
find_package(Threads REQUIRED)
target_link_libraries(rpi_smoke_test PRIVATE Threads::Threads)
