#pragma once

// A sensor that needs no hardware: temperature ramps 20..25 C, pressure is
// constant. Lets you run the whole sampling + MQTT chain on a laptop.
// LSP: substitutable for any EnvironmentSensor.

#include "station/EnvironmentSensor.hpp"

namespace station {

class FakeSensor final : public EnvironmentSensor {
public:
    bmp280::Error init() override
    {
        initialised_ = true;
        return bmp280::Error::None;
    }

    bmp280::Error readForced(bmp280::Measurement& out) override
    {
        if (!initialised_) {
            return bmp280::Error::NotInitialised;
        }
        out.temperatureC = 20.0F + static_cast<float>(tick_ % 50) * 0.1F;
        out.pressurePa   = 101325.0F;
        ++tick_;
        return bmp280::Error::None;
    }

private:
    bool     initialised_ = false;
    unsigned tick_        = 0;
};

} // namespace station
