/**
 * @file I2CHelper.cpp
 * @brief Implementation of the I2CHelper class.
 * @author J.A. Korten / johan.korten@han.nl
 * @version 1.0
 * @date 2024-09-16 
 */

#include "I2CHelper.h"

// Constructor
I2CHelper::I2CHelper(TwoWire *wire) : _wire(wire) {}


// Templated register access functions implementation
template <typename T>
T I2CHelper::readRegister(uint8_t address, uint16_t reg) {
    _wire->beginTransmission(address);
    _wire->write(reg >> 8); // MSB
    _wire->write(reg & 0xFF); // LSB
    _wire->endTransmission();

    _wire->requestFrom(address, sizeof(T));
    T value = 0;
    for (size_t i = 0; i < sizeof(T); ++i) {
        value <<= 8;
        value |= _wire->read();
    }
    return value;
}

template <typename T>
void I2CHelper::writeRegister(uint8_t address, uint16_t reg, T value) {
    _wire->beginTransmission(address);
    _wire->write(reg >> 8); // MSB
    _wire->write(reg & 0xFF); // LSB
    for (size_t i = 0; i < sizeof(T); ++i) {
        _wire->write((value >> (8 * (sizeof(T) - 1 - i))) & 0xFF);
    }
    _wire->endTransmission();
}

// Explicit instantiations for uint8_t and uint16_t
template uint8_t I2CHelper::readRegister<uint8_t>(uint8_t, uint16_t); 
template void I2CHelper::writeRegister<uint8_t>(uint8_t, uint16_t, uint8_t);
template void I2CHelper::writeRegister<uint16_t>(uint8_t, uint16_t, uint16_t);

