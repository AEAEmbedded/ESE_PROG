#include "SHT31.h"
#include <iostream>

int main() {
    SHT31 sensor;
    if (!sensor.begin()) {
        std::cerr << "Failed to initialize sensor." << std::endl;
        return 1;
    }

    float temperature, humidity;
    if (sensor.readSensor(temperature, humidity)) {
        std::cout << "Temperature: " << temperature << " C" << std::endl;
        std::cout << "Humidity: " << humidity << " %" << std::endl;
    } else {
        std::cerr << "Failed to read from sensor." << std::endl;
    }

    return 0;
}
