# CMakeLists.txt - Stage 4: External Dependencies
# CMake Workshop - HAN University of Applied Sciences
# Johan Korten - v1.0 2025
#
# STAGE 4: External Dependencies (Automatic Dependency Management)
# KEY IMPROVEMENT: FetchContent for external libraries

cmake_minimum_required(VERSION 3.20)
project(SensorApp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # For VS Code IntelliSense

# External dependencies with FetchContent
include(FetchContent)

# Add fmt library for better output formatting
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 10.1.1
)
FetchContent_MakeAvailable(fmt)

# Create sensor library
add_library(sht45_lib STATIC
    sht45.cpp
    i2c.cpp
)
target_include_directories(sht45_lib PUBLIC include)
target_compile_options(sht45_lib PRIVATE -Wall -Wextra -O2)

# Create utilities library
add_library(utils_lib STATIC
    logger.cpp
    config.cpp
    filter.cpp
)
target_include_directories(utils_lib PUBLIC include)
target_compile_options(utils_lib PRIVATE -Wall -Wextra -O2)
target_link_libraries(utils_lib PUBLIC fmt::fmt)  # Logger can use fmt!

# Create main executable
add_executable(sensor_app main.cpp)
target_link_libraries(sensor_app
    PRIVATE
        sht45_lib
        utils_lib
)
target_compile_options(sensor_app PRIVATE -Wall -Wextra -O2)

# Build type configuration
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Print build information
message(STATUS "Building ${PROJECT_NAME} version ${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "External dependencies: fmt library")
message(STATUS "CMake will download and build dependencies automatically!")
