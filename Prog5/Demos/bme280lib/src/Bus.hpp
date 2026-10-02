#pragma once

#include <cstddef>
#include <cstdint>

namespace bme280 {

/// Byte transport to one device on an I2C (or SPI) bus.
///
/// The bus knows the device address; the sensor knows the registers. Neither
/// knows the platform. Implementations: ArduinoI2cBus, LinuxI2cBus, MockBus.
class Bus {
public:
    virtual ~Bus() = default;

    /// Read `len` bytes starting at register `reg` in one transaction
    /// (register write, repeated start, read). True on success.
    [[nodiscard]] virtual bool read(uint8_t reg, uint8_t* data, size_t len) = 0;

    /// Write one byte to register `reg`. True on success.
    [[nodiscard]] virtual bool write(uint8_t reg, uint8_t value) = 0;
};

} // namespace bme280
