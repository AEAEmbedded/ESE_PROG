# CMakeLists.txt - MqttSkeleton (C++) root
# ESE Health Programming Challenge 2026-2027 - HAN University of Applied Sciences
# Johan Korten - v1.0 2026
#
# Same layout as the ChallengeSkeleton of the CMake workshop, now with an
# MQTT layer:
#   mqtt/     - topic set, payload helpers and the IMessageBus interface
#               (pure logic, no I/O) plus MosquittoBus (libmosquitto)
#   devices/  - VirtualThermometer: a SimDevice that only knows IMessageBus
#   app/      - the executable that wires MosquittoBus and the thermometer
#   tests/    - CppUTest unit tests, run without a broker (FakeBus)
#
# Dependency: libmosquitto (apt install libmosquitto-dev / brew install mosquitto)

cmake_minimum_required(VERSION 3.20)
project(MqttSkeleton VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(MQTT_SKELETON_BUILD_TESTS "Build the unit tests (downloads CppUTest)" ON)

add_subdirectory(mqtt)
add_subdirectory(devices)
add_subdirectory(app)

if(MQTT_SKELETON_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
