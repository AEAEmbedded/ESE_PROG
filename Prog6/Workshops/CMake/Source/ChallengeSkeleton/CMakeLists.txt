# CMakeLists.txt - ChallengeSkeleton (root)
# CMake Workshop - HAN University of Applied Sciences
# Johan Korten - v1.0 2026
#
# Starting-point structure for the final challenge:
#   core/   - pure logic as a library (testable, no I/O)
#   app/    - the executable, links against the core library
#   tests/  - CppUTest unit tests, run via ctest

cmake_minimum_required(VERSION 3.20)
project(ChallengeSkeleton VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # For VS Code IntelliSense

add_subdirectory(core)
add_subdirectory(app)

enable_testing()
add_subdirectory(tests)
