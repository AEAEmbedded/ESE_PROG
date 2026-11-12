// config.cpp
#include "config.hpp"

Config::Config() {
    // RAII: Configuration loaded on construction
    // In real implementation, would read from file/EEPROM
}

int32_t Config::getSampleRate() const {
    return DEFAULT_SAMPLE_RATE_MS;
}

bool Config::isDebugMode() const {
    return DEFAULT_DEBUG_MODE;
}
