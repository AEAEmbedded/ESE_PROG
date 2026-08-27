#ifndef TEMPERATURE_CONVERSION_HPP
#define TEMPERATURE_CONVERSION_HPP

#include <cstdint>

namespace temperature {  // <-- This line

struct SensorConfig {
    static constexpr uint16_t ADC_MAX_VALUE = 4095U;
    static constexpr float TEMP_MIN_CELSIUS = -40.0F;
    static constexpr float TEMP_MAX_CELSIUS = 85.0F;
    static constexpr float TEMP_RANGE_CELSIUS = TEMP_MAX_CELSIUS - TEMP_MIN_CELSIUS;
};

float adcToCelsius(uint16_t adcValue);
bool isInValidRange(float celsius);
float celsiusToFahrenheit(float celsius);

}  // namespace temperature  // <-- And this line

#endif