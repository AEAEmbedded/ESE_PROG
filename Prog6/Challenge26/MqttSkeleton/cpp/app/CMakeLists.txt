add_executable(virtual_thermometer main.cpp)
target_link_libraries(virtual_thermometer PRIVATE sim_devices sim_bus_mosquitto)
target_compile_options(virtual_thermometer PRIVATE -Wall -Wextra -Wpedantic)
