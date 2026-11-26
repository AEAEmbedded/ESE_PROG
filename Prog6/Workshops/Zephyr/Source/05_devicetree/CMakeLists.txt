# Zephyr Workshop - 05_devicetree
# HAN University of Applied Sciences - Johan Korten - v1.0 2026

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(devicetree_workshop)

target_sources(app PRIVATE src/main.c)
