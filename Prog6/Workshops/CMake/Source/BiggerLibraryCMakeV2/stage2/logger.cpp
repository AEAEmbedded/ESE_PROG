// logger.cpp
#include "logger.hpp"
#include <iostream>

void Logger::log(const std::string& message) const {
    std::cout << "[LOG] " << message << std::endl;
}

void Logger::error(const std::string& message) const {
    std::cerr << "[ERROR] " << message << std::endl;
}
