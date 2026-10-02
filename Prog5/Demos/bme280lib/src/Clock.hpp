#pragma once

#include <cstdint>

namespace bme280 {

/// Time, separated from the bus (Interface Segregation): the sensor needs to
/// wait for a measurement, a bus never does. Implementations: ArduinoClock,
/// LinuxClock, FakeClock.
class Clock {
public:
    virtual ~Clock() = default;

    /// Block for at least `us` microseconds.
    virtual void delayUs(uint32_t us) = 0;
};

} // namespace bme280
