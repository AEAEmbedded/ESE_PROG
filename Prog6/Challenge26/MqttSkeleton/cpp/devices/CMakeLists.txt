# SimDevices. They know IMessageBus, topics and payloads (sim_bus), never
# libmosquitto. That is what makes them testable without a broker.
add_library(sim_devices STATIC
    virtual_thermometer.cpp
)
target_include_directories(sim_devices PUBLIC include)
target_link_libraries(sim_devices PUBLIC sim_bus)
target_compile_options(sim_devices PRIVATE -Wall -Wextra -Wpedantic)
