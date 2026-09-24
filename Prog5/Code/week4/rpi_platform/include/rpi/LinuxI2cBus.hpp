#pragma once

// Raspberry Pi implementation of the Bus interface from the week-2 starter,
// on top of the Linux i2c-dev driver (/dev/i2c-1).
//
// This file is the *only* place in the Pi build that knows about Linux. The
// sensor library (include/, src/) never includes it: it sees a Bus&.
//
// Adapt the include and base class to your own library (bme280::Bus, ...).

#include "bme280/Bus.hpp"

#include <cstddef>
#include <cstdint>

// NOTE: `linux` is a predefined macro in GCC, so `namespace linux` does not
// compile. We use `rpi`.
namespace rpi {

class LinuxI2cBus final : public bme280::Bus {
public:
    /// Opens `device` (e.g. "/dev/i2c-1") for the slave at 7-bit `address`.
    /// Check isOpen() afterwards; the constructor does not throw.
    LinuxI2cBus(const char* device, uint8_t address);

    /// Closes the device. RAII: whoever owns the object owns the descriptor.
    ~LinuxI2cBus() override;

    // Non-copyable: two objects closing the same file descriptor is a bug.
    LinuxI2cBus(const LinuxI2cBus&)            = delete;
    LinuxI2cBus& operator=(const LinuxI2cBus&) = delete;

    bool isOpen() const { return fd_ >= 0; }

    /// One I2C transaction: write `reg`, repeated start, read `len` bytes.
    bool read(uint8_t reg, uint8_t* data, size_t len) override;

    /// One I2C transaction: write `reg` followed by `len` bytes.
    bool write(uint8_t reg, const uint8_t* data, size_t len) override;

    /// Sleeps at least `us` microseconds.
    ///
    /// ISP smell, on purpose: a bus has nothing to do with time, yet every Bus
    /// implementation (Arduino, Linux, Mock) is forced to provide this.
    /// Assignment 4, step 3: move it to a separate Clock interface.
    void delayUs(uint32_t us) override;

private:
    int     fd_ = -1;
    uint8_t address_;
};

} // namespace rpi
