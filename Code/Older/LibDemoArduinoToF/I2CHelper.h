/**
 * @file I2CHelper.h
 * @brief Helper class for I2C register access.
 * @author J.A. Korten / johan.korten@han.nl
 * @version 1.0
 * @date 2024-09-16 
 */
 
 #ifndef I2CHELPER_H
#define I2CHELPER_H

#include <Wire.h>

class I2CHelper {
public:
    // Constructor taking the TwoWire instance
    I2CHelper(TwoWire *wire); 

    // Templated register access functions
    template <typename T>
    T readRegister(uint8_t address, uint16_t reg);

    template <typename T>
    void writeRegister(uint8_t address, uint16_t reg, T value);

private:
    TwoWire *_wire;
};

#endif