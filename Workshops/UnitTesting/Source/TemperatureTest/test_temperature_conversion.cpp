#include "CppUTest/TestHarness.h"
#include "temperature_conversion.hpp"

using namespace temperature;

// ============================================================================
// ADC to Celsius Conversion
// ============================================================================

TEST_GROUP(AdcToCelsius) {
};

TEST(AdcToCelsius, MinimumAdcValueGivesMinimumTemperature) {
    constexpr uint16_t ADC_MIN = 0U;
    
    DOUBLES_EQUAL(SensorConfig::TEMP_MIN_CELSIUS, adcToCelsius(ADC_MIN), 0.1);
}

TEST(AdcToCelsius, MaximumAdcValueGivesMaximumTemperature) {
    DOUBLES_EQUAL(SensorConfig::TEMP_MAX_CELSIUS, 
                  adcToCelsius(SensorConfig::ADC_MAX_VALUE), 
                  0.1);
}

TEST(AdcToCelsius, MidpointAdcValueGivesMidpointTemperature) {
    constexpr uint16_t ADC_MIDPOINT = SensorConfig::ADC_MAX_VALUE / 2U;
    constexpr float TEMP_MIDPOINT = (SensorConfig::TEMP_MIN_CELSIUS 
                                   + SensorConfig::TEMP_MAX_CELSIUS) / 2.0F;
    
    DOUBLES_EQUAL(TEMP_MIDPOINT, adcToCelsius(ADC_MIDPOINT), 0.5);
}

// TODO: Students implement
// TEST(AdcToCelsius, RoomTemperatureAdcValue)
// Hint: What ADC value corresponds to approximately 20°C?

// ============================================================================
// Temperature Range Validation
// ============================================================================

TEST_GROUP(IsInValidRange) {
};

TEST(IsInValidRange, TemperatureWithinRangeIsValid) {
    constexpr float ROOM_TEMPERATURE = 20.0F;
    
    CHECK_TRUE(isInValidRange(ROOM_TEMPERATURE));
}

TEST(IsInValidRange, MinimumBoundaryIsValid) {
    CHECK_TRUE(isInValidRange(SensorConfig::TEMP_MIN_CELSIUS));
}

TEST(IsInValidRange, MaximumBoundaryIsValid) {
    CHECK_TRUE(isInValidRange(SensorConfig::TEMP_MAX_CELSIUS));
}

// TODO: Students implement
// TEST(IsInValidRange, BelowMinimumIsInvalid)
// TEST(IsInValidRange, AboveMaximumIsInvalid)

// ============================================================================
// Celsius to Fahrenheit Conversion
// ============================================================================

TEST_GROUP(CelsiusToFahrenheit) {
};

TEST(CelsiusToFahrenheit, WaterFreezingPoint) {
    constexpr float WATER_FREEZING_CELSIUS = 0.0F;
    constexpr float WATER_FREEZING_FAHRENHEIT = 32.0F;
    
    DOUBLES_EQUAL(WATER_FREEZING_FAHRENHEIT, 
                  celsiusToFahrenheit(WATER_FREEZING_CELSIUS), 
                  0.1);
}

TEST(CelsiusToFahrenheit, WaterBoilingPoint) {
    constexpr float WATER_BOILING_CELSIUS = 100.0F;
    constexpr float WATER_BOILING_FAHRENHEIT = 212.0F;
    
    DOUBLES_EQUAL(WATER_BOILING_FAHRENHEIT, 
                  celsiusToFahrenheit(WATER_BOILING_CELSIUS), 
                  0.1);
}

// TODO: Students implement
// TEST(CelsiusToFahrenheit, HumanBodyTemperature)
// TEST(CelsiusToFahrenheit, NegativeTemperature)