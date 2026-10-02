#pragma once
#ifdef __linux__

#include "Bus.hpp"
#include "Clock.hpp"

namespace bme280 {

/// Bus on Linux i2c-dev (/dev/i2c-N). Owns the file descriptor: opened in
/// the constructor, closed in the destructor, never copied (RAII).
class LinuxI2cBus final : public Bus {
public:
    LinuxI2cBus(const char* device, uint8_t address);
    ~LinuxI2cBus() override;

    LinuxI2cBus(const LinuxI2cBus&)            = delete;
    LinuxI2cBus& operator=(const LinuxI2cBus&) = delete;

    [[nodiscard]] bool isOpen() const { return fd_ >= 0; }

    bool read(uint8_t reg, uint8_t* data, size_t len) override;
    bool write(uint8_t reg, uint8_t value) override;

private:
    int     fd_ = -1;
    uint8_t address_;
};

class LinuxClock final : public Clock {
public:
    void delayUs(uint32_t us) override;
};

} // namespace bme280

#endif // __linux__
