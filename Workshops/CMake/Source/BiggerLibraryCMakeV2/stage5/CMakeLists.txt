# CMakeLists.txt - Stage 5: Professional Version
# CMake Workshop - HAN University of Applied Sciences
# Johan Korten - v1.0 2025
#
# STAGE 5: Professional Quality (Production-Ready)
# This is how real projects configure CMake!

cmake_minimum_required(VERSION 3.20)

# Project definition with full metadata
project(SensorApp
    VERSION 1.0.0
    DESCRIPTION "SHT45 Temperature/Humidity Sensor Monitor"
    HOMEPAGE_URL "https://github.com/han-university/sensor-app"
    LANGUAGES CXX
)

# Global C++ settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build type with sensible default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    message(STATUS "Build type not specified, defaulting to Release")
endif()

# Compiler-specific warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(/W4)
endif()

# Project options (user-configurable)
option(BUILD_TESTS "Build unit tests" OFF)
option(USE_FMT "Use fmt library for formatting" ON)
option(ENABLE_INSTALL "Enable installation rules" ON)

# External dependencies (conditional)
if(USE_FMT)
    include(FetchContent)
    message(STATUS "Fetching fmt library...")
    FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 10.1.1
    )
    FetchContent_MakeAvailable(fmt)
endif()

#
# Sensor library (core hardware functionality)
#
add_library(sht45 STATIC
    sht45.cpp
    i2c.cpp
)

# Generator expressions for build/install include paths
target_include_directories(sht45
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Set library properties
set_target_properties(sht45 PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    OUTPUT_NAME "sensor_sht45"
)

#
# Utilities library
#
add_library(utils STATIC
    logger.cpp
    config.cpp
    filter.cpp
)

target_include_directories(utils
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

set_target_properties(utils PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    OUTPUT_NAME "sensor_utils"
)

# Link fmt if enabled
if(USE_FMT)
    target_link_libraries(utils PUBLIC fmt::fmt)
    target_compile_definitions(utils PUBLIC USE_FMT)
endif()

#
# Main application executable
#
add_executable(${PROJECT_NAME} main.cpp)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        sht45
        utils
)

# Set executable properties
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "sensor_app"
    VERSION ${PROJECT_VERSION}
)

#
# Installation rules
#
if(ENABLE_INSTALL)
    include(GNUInstallDirs)

    # Install executable
    install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT Runtime
    )

    # Install libraries
    install(TARGETS sht45 utils
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT Development
    )

    # Install headers
    install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
        COMPONENT Development
        FILES_MATCHING PATTERN "*.hpp"
    )

    # Install README and LICENSE (if they exist)
    install(FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/README.md
        DESTINATION ${CMAKE_INSTALL_DOCDIR}
        COMPONENT Documentation
        OPTIONAL
    )
endif()

#
# Testing support
#
if(BUILD_TESTS)
    enable_testing()
    message(STATUS "Building with tests enabled")
    # add_subdirectory(tests)  # Uncomment when tests exist
endif()

#
# Build configuration summary
#
message(STATUS "")
message(STATUS "=================================")
message(STATUS "${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "=================================")
message(STATUS "Build type:        ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard:      C++${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler:          ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Build tests:       ${BUILD_TESTS}")
message(STATUS "Use fmt:           ${USE_FMT}")
message(STATUS "Enable install:    ${ENABLE_INSTALL}")
message(STATUS "Install prefix:    ${CMAKE_INSTALL_PREFIX}")
message(STATUS "=================================")
message(STATUS "")
