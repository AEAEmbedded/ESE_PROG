# CMakeLists.txt - Stage 2: Modern CMake Basics
# CMake Workshop - HAN University of Applied Sciences
# Johan Korten - v1.0 2025
#
# STAGE 2: Modern Basics (Target-Specific Commands)
# KEY IMPROVEMENT: Use target-specific commands with PRIVATE keyword

cmake_minimum_required(VERSION 3.20)  # Updated for modern features
project(SensorApp VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Source files
set(SOURCES
    main.cpp
    sht45.cpp
    i2c.cpp
    logger.cpp
    config.cpp
    filter.cpp
)

# Create executable
add_executable(sensor_app ${SOURCES})

# MODERN: Use target-specific commands with PRIVATE visibility
# This means ONLY sensor_app uses these settings, not everything in the project!
target_include_directories(sensor_app PRIVATE include)
target_compile_options(sensor_app PRIVATE -Wall -Wextra -O2)

# Print build information
message(STATUS "Building ${PROJECT_NAME} version ${PROJECT_VERSION}")
message(STATUS "C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "Using modern target-specific commands!")
