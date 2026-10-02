#pragma once
#ifdef ARDUINO

#include "Bus.hpp"
#include "Clock.hpp"

#include <Arduino.h>
#include <Wire.h>

namespace bme280 {

/// Bus on an Arduino TwoWire. The sketch owns and begins the TwoWire; this
/// class only uses it, so two sensors can share a bus and a second bus
/// (Wire1, or TwoWire(PB11, PB10) on STM32) is just another object.
class ArduinoI2cBus final : public Bus {
public:
    ArduinoI2cBus(TwoWire& wire, uint8_t address) : wire_(wire), address_(address) {}

    bool read(uint8_t reg, uint8_t* data, size_t len) override;
    void useRepeatedStart(bool on) { repeatedStart_ = on; }
    bool write(uint8_t reg, uint8_t value) override;

private:
    TwoWire& wire_;
    uint8_t  address_;
    bool     repeatedStart_ = true;
};

class ArduinoClock final : public Clock {
public:
    void delayUs(uint32_t us) override
    {
        // delayMicroseconds() is limited to 16383 on AVR; STM32 handles more,
        // but ms + us keeps it portable and keeps yield() alive on cores that need it.
        if (us >= 1000) { delay(us / 1000); us %= 1000; }
        if (us > 0) delayMicroseconds(us);
    }
};

} // namespace bme280

#endif // ARDUINO
