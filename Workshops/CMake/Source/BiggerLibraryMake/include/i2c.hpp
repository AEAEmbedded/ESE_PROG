// i2c.hpp
#pragma once

#include <cstdint>

/// @brief I2C Communication Interface
/// @details Follows RAII - bus is initialized on construction
class I2C {
public:
    /// @brief Initialize I2C bus on construction
    I2C();
    ~I2C() = default;

    // Explicitly default copy/move operations (Rule of Five)
    I2C(const I2C&) = default;
    I2C& operator=(const I2C&) = default;
    I2C(I2C&&) = default;
    I2C& operator=(I2C&&) = default;

    /// @brief Write single byte to I2C bus
    /// @param data Byte to write
    void write(uint8_t data);

    /// @brief Read single byte from I2C bus
    /// @return Byte read from bus
    uint8_t read() const;

private:
    static constexpr uint8_t SIMULATED_READ_VALUE = 0x42U;
};
