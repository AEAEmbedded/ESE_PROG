#include "temperature_conversion.hpp"

namespace temperature {

float adcToCelsius(uint16_t adcValue) {
    const float normalizedValue = static_cast<float>(adcValue) 
                                / static_cast<float>(SensorConfig::ADC_MAX_VALUE);
    
    return (normalizedValue * SensorConfig::TEMP_RANGE_CELSIUS) 
           + SensorConfig::TEMP_MIN_CELSIUS;
}

bool isInValidRange(float celsius) {
    return (celsius >= SensorConfig::TEMP_MIN_CELSIUS) 
        && (celsius <= SensorConfig::TEMP_MAX_CELSIUS);
}

float celsiusToFahrenheit(float celsius) {
    constexpr float SCALE_FACTOR = 9.0F / 5.0F;
    constexpr float OFFSET = 32.0F;
    
    return (celsius * SCALE_FACTOR) + OFFSET;
}

}  // namespace temperature