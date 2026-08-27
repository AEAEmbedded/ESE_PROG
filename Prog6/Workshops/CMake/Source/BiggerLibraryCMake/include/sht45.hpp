// sht45.hpp
#pragma once

#include <cstdint>

/// @brief SHT45 Temperature and Humidity Sensor Interface
/// @details Follows RAII - sensor is ready after construction
class SHT45 {
public:
    SHT45() = default;
    ~SHT45() = default;

    // Explicitly default copy/move operations (Rule of Five)
    SHT45(const SHT45&) = default;
    SHT45& operator=(const SHT45&) = default;
    SHT45(SHT45&&) = default;
    SHT45& operator=(SHT45&&) = default;

    /// @brief Read temperature from sensor
    /// @return Temperature in degrees Celsius
    float read_temperature() const;

private:
    static constexpr float SIMULATED_TEMPERATURE = 22.5F;
};
