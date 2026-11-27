# Two libraries, on purpose:
#   sim_bus            - topics, payloads, IMessageBus. No I/O, no external
#                        dependency, so every component and every test can
#                        use it.
#   sim_bus_mosquitto  - the one place that knows libmosquitto.
add_library(sim_bus STATIC
    payload.cpp
)
target_include_directories(sim_bus PUBLIC include)
target_compile_options(sim_bus PRIVATE -Wall -Wextra -Wpedantic)

find_package(PkgConfig REQUIRED)
pkg_check_modules(MOSQUITTO REQUIRED IMPORTED_TARGET libmosquitto)

add_library(sim_bus_mosquitto STATIC
    mosquitto_bus.cpp
)
target_link_libraries(sim_bus_mosquitto PUBLIC sim_bus PkgConfig::MOSQUITTO)
target_compile_options(sim_bus_mosquitto PRIVATE -Wall -Wextra -Wpedantic)
