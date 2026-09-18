#pragma once

// What the station sees of a sensor. Nothing about I2C, Bosch or BME280.
// DIP: Sampler depends on this, never on bmp280::Bmp280 directly.
//
// Assignment 5, step 4: let your Bme280 class implement this interface
// itself; the adapter in Bmp280Adapter.hpp is only there so the week-2
// starter works unchanged.

#include "bmp280/Types.hpp"

namespace station {

class EnvironmentSensor {
public:
    virtual ~EnvironmentSensor() = default;

    /// Bring the sensor to a state in which readForced() works.
    virtual bmp280::Error init() = 0;

    /// Take one measurement, block until it is available.
    virtual bmp280::Error readForced(bmp280::Measurement& out) = 0;
};

} // namespace station
