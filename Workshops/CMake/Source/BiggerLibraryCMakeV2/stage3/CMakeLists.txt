# CMakeLists.txt - Stage 3: Library Structure
# CMake Workshop - HAN University of Applied Sciences
# Johan Korten - v1.0 2025
#
# STAGE 3: Library Structure (Modular Architecture)
# KEY IMPROVEMENT: Separate libraries for reusability

cmake_minimum_required(VERSION 3.20)
project(SensorApp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Create sensor hardware library (SHT45 + I2C)
add_library(sht45_lib STATIC
    sht45.cpp
    i2c.cpp
)
target_include_directories(sht45_lib PUBLIC include)
target_compile_options(sht45_lib PRIVATE -Wall -Wextra -O2)

# Create utilities library (Logger, Config, Filter)
add_library(utils_lib STATIC
    logger.cpp
    config.cpp
    filter.cpp
)
target_include_directories(utils_lib PUBLIC include)
target_compile_options(utils_lib PRIVATE -Wall -Wextra -O2)

# Create main executable
add_executable(sensor_app main.cpp)

# Link libraries to executable
target_link_libraries(sensor_app
    PRIVATE
        sht45_lib
        utils_lib
)

# Compile options for the executable
target_compile_options(sensor_app PRIVATE -Wall -Wextra -O2)

# Print build information
message(STATUS "Building ${PROJECT_NAME} version ${PROJECT_VERSION}")
message(STATUS "Libraries: sht45_lib, utils_lib")
message(STATUS "Modular architecture - libraries can be reused!")
