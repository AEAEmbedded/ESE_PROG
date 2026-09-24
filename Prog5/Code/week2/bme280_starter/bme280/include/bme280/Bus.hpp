#pragma once

#include <cstddef>
#include <cstdint>

namespace bme280 {

/// Minimal byte-transport interface the sensor needs (DIP: the driver
/// depends on this abstraction, never on Wire/HAL/Linux directly).
/// Implementations: ArduinoI2cBus, LinuxI2cBus (platform/), MockBus (tests/).
class Bus {
public:
    virtual ~Bus() = default;

    /// Read `len` bytes starting at register `reg`. Return true on success.
    virtual bool read(uint8_t reg, uint8_t* data, size_t len) = 0;

    /// Write `len` bytes starting at register `reg`. Return true on success.
    virtual bool write(uint8_t reg, const uint8_t* data, size_t len) = 0;

    /// Block for at least `us` microseconds.
    /// Assignment 4 asks you to move this into a separate Clock interface (ISP).
    virtual void delayUs(uint32_t us) = 0;
};

} // namespace bme280
