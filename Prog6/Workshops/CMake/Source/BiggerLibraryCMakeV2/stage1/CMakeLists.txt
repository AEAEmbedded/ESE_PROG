# CMakeLists.txt - Stage 1: Basic CMake
# CMake Workshop - HAN University of Applied Sciences
# Johan Korten - v1.0 2025
#
# STAGE 1: Starting Point (Basic but works)
# This is the same as BiggerLibraryCMake - functional but not modern

# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(SensorApp VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags (GLOBAL - affects everything)
add_compile_options(-Wall -Wextra -O2)

# Include directories (GLOBAL - affects everything)
include_directories(include)

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

# Print build information
message(STATUS "Building ${PROJECT_NAME} version ${PROJECT_VERSION}")
message(STATUS "C++ Standard: C++${CMAKE_CXX_STANDARD}")
