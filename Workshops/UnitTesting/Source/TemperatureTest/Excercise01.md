# Exercise 01: Temperature Sensor Library

## Goal

Learn to test pure functions using CppUTest. No classes, no mocking—just input → output.

## The Code Under Test

We have a small temperature conversion library (`temperature_conversion.hpp/cpp`) with three functions:

| Function | Purpose |
|----------|---------|
| `adcToCelsius()` | Convert 12-bit ADC value to temperature |
| `isInValidRange()` | Check if temperature is within sensor limits |
| `celsiusToFahrenheit()` | Convert Celsius to Fahrenheit |

## Your Task

Complete the TODO tests in `test_temperature_conversion.cpp`.

### Part 1: ADC Conversion
```cpp
// TODO: Students implement
// TEST(AdcToCelsius, RoomTemperatureAdcValue)
// Hint: What ADC value corresponds to approximately 20°C?
```

Use the conversion formula:
```
adcValue = (temperature - TEMP_MIN) / TEMP_RANGE * ADC_MAX
```

### Part 2: Range Validation
```cpp
// TODO: Students implement
// TEST(IsInValidRange, BelowMinimumIsInvalid)
// TEST(IsInValidRange, AboveMaximumIsInvalid)
```

Test the boundaries. What happens just outside the valid range?

### Part 3: Fahrenheit Conversion
```cpp
// TODO: Students implement
// TEST(CelsiusToFahrenheit, HumanBodyTemperature)
// TEST(CelsiusToFahrenheit, NegativeTemperature)
```

Known reference points:

| Celsius | Fahrenheit |
|---------|------------|
| 37°C | 98.6°F (body temperature) |
| -40°C | -40°F (scales meet) |

## CppUTest Quick Reference
```cpp
// Integer comparison
LONGS_EQUAL(expected, actual);

// Floating point comparison (with tolerance)
DOUBLES_EQUAL(expected, actual, tolerance);

// Boolean checks
CHECK_TRUE(condition);
CHECK_FALSE(condition);

// Use constants from SensorConfig
SensorConfig::ADC_MAX_VALUE      // 4095
SensorConfig::TEMP_MIN_CELSIUS   // -40.0F
SensorConfig::TEMP_MAX_CELSIUS   // 85.0F
```

## Build and Run
```bash
cmake -B build
cmake --build build
./build/test_temperature_conversion
```

Expected output when all tests pass:
```
..........
OK (13 tests, 13 ran, 13 checks, 0 ignored, 0 filtered out, 0 ms)
```

## Discussion Questions

1. Why do we use `DOUBLES_EQUAL` with a tolerance instead of exact comparison?
2. What makes these functions easy to test?
3. How would testing change if `adcToCelsius()` read directly from hardware?
