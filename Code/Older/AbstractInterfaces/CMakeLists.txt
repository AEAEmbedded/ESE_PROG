cmake_minimum_required(VERSION 3.10)
project(AbstractInterfaces)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main
    main.cpp
    Dog.cpp
    Fish.cpp
    GermanShepherd.cpp
    PoliceDog.cpp
)

target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})