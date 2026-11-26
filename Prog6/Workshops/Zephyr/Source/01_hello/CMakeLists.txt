# Zephyr Workshop - 01_hello
# HAN University of Applied Sciences - Johan Korten - v1.0 2026
#
# A Zephyr application is a normal CMake project. find_package(Zephyr)
# pulls in the entire RTOS build; you only add your own sources to the
# `app` target that Zephyr defines.

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(hello_workshop)

target_sources(app PRIVATE src/main.c)
