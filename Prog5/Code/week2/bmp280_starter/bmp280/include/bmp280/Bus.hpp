#pragma once

#include <cstddef>
#include <cstdint>

namespace bmp280 {

/// Minimal byte-transport interface the sensor needs (DIP: the driver
/// depends on this abstraction, never on Wire/HAL/Zephyr directly).
/// Implementations: I2cBus, SpiBus (examples/), MockBus (tests/).
class Bus {
public:
    virtual ~Bus() = default;

    /// Read `len` bytes starting at register `reg`. Return true on success.
    virtual bool read(uint8_t reg, uint8_t* data, size_t len) = 0;

    /// Write `len` bytes starting at register `reg`. Return true on success.
    virtual bool write(uint8_t reg, const uint8_t* data, size_t len) = 0;

    /// Block for at least `us` microseconds.
    virtual void delayUs(uint32_t us) = 0;
};

} // namespace bmp280
