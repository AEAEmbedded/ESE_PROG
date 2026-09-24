#pragma once

// Adapter: makes the week-2 starter class look like an EnvironmentSensor
// without changing it. Once Bme280 implements EnvironmentSensor directly,
// delete this file.

#include "bme280/Bme280.hpp"
#include "station/EnvironmentSensor.hpp"

namespace station {

class Bme280Adapter final : public EnvironmentSensor {
public:
    explicit Bme280Adapter(bme280::Bme280& sensor) : sensor_(sensor) {}

    bme280::Error init() override { return sensor_.init(); }
    bme280::Error readForced(bme280::Measurement& out) override { return sensor_.readForced(out); }

private:
    bme280::Bme280& sensor_;
};

} // namespace station
