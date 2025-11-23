// Our starter project
// Cmake Workshop Prog 6 - Step 6
//
// HAN University of Applied Sciences
// Johan Korten - v1.0 2025
//
// main.cpp
// compile using:
// g++ main.cpp sht45.cpp i2c.cpp logger.cpp config.cpp filter.cpp -I./include -std=c++17 -Wall -O2 -o sensor_app

#include <iostream>
#include "sht45.hpp"
#include "i2c.hpp"
#include "logger.hpp"
#include "config.hpp"
#include "filter.hpp"

int main() {
    Logger logger;
    Config config;      // Config loaded in constructor
    I2C i2c;           // I2C bus initialized in constructor
    Filter filter;
    SHT45 sensor;

    logger.log("Starting sensor application...");

    float temp = sensor.read_temperature();
    float filtered_temp = filter.apply(temp);

    std::cout << "Temperature: " << filtered_temp << std::endl;
    logger.log("Application complete");

    return 0;
}