#pragma once

// Adapter: makes the week-2 starter class look like an EnvironmentSensor
// without changing it. Once Bme280 implements EnvironmentSensor directly,
// delete this file.

#include "bmp280/Bmp280.hpp"
#include "station/EnvironmentSensor.hpp"

namespace station {

class Bmp280Adapter final : public EnvironmentSensor {
public:
    explicit Bmp280Adapter(bmp280::Bmp280& sensor) : sensor_(sensor) {}

    bmp280::Error init() override { return sensor_.init(); }
    bmp280::Error readForced(bmp280::Measurement& out) override { return sensor_.readForced(out); }

private:
    bmp280::Bmp280& sensor_;
};

} // namespace station
