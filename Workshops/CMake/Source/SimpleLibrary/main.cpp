// Our starter project
// Cmake Workshop Prog 6 - Step 2
//
// HAN University of Applied Sciences
// Johan Korten - v1.0 2025
//
// main.cpp
// 

#include <iostream>
#include "sht45.hpp"

int main() {
    SHT45 sensor;
    std::cout << "Temperature: " << sensor.read_temperature() << std::endl;
    return 0;
}