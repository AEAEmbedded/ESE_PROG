#pragma once

// What the station sees of a sensor. Nothing about I2C, Bosch or BME280.
// DIP: Sampler depends on this, never on bme280::Bme280 directly.
//
// Assignment 5, step 4: let your Bme280 class implement this interface
// itself; the adapter in Bme280Adapter.hpp is only there so the week-2
// starter works unchanged.

#include "bme280/Types.hpp"

namespace station {

class EnvironmentSensor {
public:
    virtual ~EnvironmentSensor() = default;

    /// Bring the sensor to a state in which readForced() works.
    virtual bme280::Error init() = 0;

    /// Take one measurement, block until it is available.
    virtual bme280::Error readForced(bme280::Measurement& out) = 0;
};

} // namespace station
