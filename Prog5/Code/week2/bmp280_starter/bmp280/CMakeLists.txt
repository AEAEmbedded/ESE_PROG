cmake_minimum_required(VERSION 3.16)
project(bmp280 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BMP280_BUILD_TESTS "Build host-side unit tests" OFF)

# --- Vendored Bosch BMP2 SensorAPI (drop bmp2.c / bmp2.h / bmp2_defs.h here)
add_library(bmp2_bosch STATIC third_party/bmp2/bmp2.c)
target_include_directories(bmp2_bosch PUBLIC third_party/bmp2)
# Integer compensation for MCUs without FPU; remove to use double.
target_compile_definitions(bmp2_bosch PUBLIC BMP2_32BIT_COMPENSATION)

# --- The wrapper library: depends only on Bus, never on a HAL
add_library(bmp280 STATIC src/Bmp280.cpp)
target_include_directories(bmp280 PUBLIC include)
target_link_libraries(bmp280 PUBLIC bmp2_bosch)
target_compile_options(bmp280 PRIVATE -Wall -Wextra -Wpedantic)

if(BMP280_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
