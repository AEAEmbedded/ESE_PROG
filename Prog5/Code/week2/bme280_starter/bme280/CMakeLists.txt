cmake_minimum_required(VERSION 3.16)
project(bme280 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BME280_BUILD_TESTS "Build host-side unit tests" OFF)

# --- Vendored Bosch BME280 SensorAPI (third_party/bme280) ----------------------
add_library(bme280_bosch STATIC third_party/bme280/bme280.c)
target_include_directories(bme280_bosch PUBLIC third_party/bme280)
# Integer compensation for MCUs without FPU; remove to use double.
target_compile_definitions(bme280_bosch PUBLIC BME280_32BIT_ENABLE)

# --- The wrapper library: depends only on Bus, never on a HAL -----------------
add_library(bme280 STATIC src/Bme280.cpp)
target_include_directories(bme280 PUBLIC include)
target_link_libraries(bme280 PUBLIC bme280_bosch)
target_compile_options(bme280 PRIVATE -Wall -Wextra -Wpedantic)

if(BME280_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
