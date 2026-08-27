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

TEST(AdcToCelsius, RoomTemperatureAdcValue) {
    // Room temperature ~20°C
    // Formula: adcValue = (temp - TEMP_MIN) / TEMP_RANGE * ADC_MAX
    // adcValue = (20 - (-40)) / 125 * 4095 = 60/125 * 4095 ≈ 1966
    constexpr uint16_t ADC_ROOM_TEMP = 1966U;
    constexpr float EXPECTED_ROOM_TEMP = 20.0F;
    
    DOUBLES_EQUAL(EXPECTED_ROOM_TEMP, adcToCelsius(ADC_ROOM_TEMP), 0.5);
}

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

TEST(IsInValidRange, BelowMinimumIsInvalid) {
    constexpr float BELOW_MINIMUM = SensorConfig::TEMP_MIN_CELSIUS - 1.0F;
    
    CHECK_FALSE(isInValidRange(BELOW_MINIMUM));
}

TEST(IsInValidRange, AboveMaximumIsInvalid) {
    constexpr float ABOVE_MAXIMUM = SensorConfig::TEMP_MAX_CELSIUS + 1.0F;
    
    CHECK_FALSE(isInValidRange(ABOVE_MAXIMUM));
}

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

TEST(CelsiusToFahrenheit, HumanBodyTemperature) {
    constexpr float BODY_TEMP_CELSIUS = 37.0F;
    constexpr float BODY_TEMP_FAHRENHEIT = 98.6F;
    
    DOUBLES_EQUAL(BODY_TEMP_FAHRENHEIT, 
                  celsiusToFahrenheit(BODY_TEMP_CELSIUS), 
                  0.1);
}

TEST(CelsiusToFahrenheit, NegativeTemperature) {
    constexpr float NEGATIVE_CELSIUS = -40.0F;
    constexpr float NEGATIVE_FAHRENHEIT = -40.0F;  // Where scales meet
    
    DOUBLES_EQUAL(NEGATIVE_FAHRENHEIT, 
                  celsiusToFahrenheit(NEGATIVE_CELSIUS), 
                  0.1);
}