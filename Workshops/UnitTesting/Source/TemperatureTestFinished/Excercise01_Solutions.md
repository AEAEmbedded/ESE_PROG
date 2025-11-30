# Exercise 01: Temperature Sensor Library (Complete)

## Goal

Learn to test pure functions using CppUTest. No classes, no mocking—just input → output.

## The Code Under Test

A small temperature conversion library (`temperature_conversion.hpp/cpp`) with three functions:

| Function | Purpose |
|----------|---------|
| `adcToCelsius()` | Convert 12-bit ADC value to temperature |
| `isInValidRange()` | Check if temperature is within sensor limits |
| `celsiusToFahrenheit()` | Convert Celsius to Fahrenheit |

## File Structure
```
01_temperature_conversion/
├── CMakeLists.txt
├── temperature_conversion.hpp
├── temperature_conversion.cpp
├── test_temperature_conversion.cpp
└── main.cpp
```

## Build and Run
```bash
cmake -B build
cmake --build build
./build/test_temperature_conversion
```

Expected output:
```
.............
OK (13 tests, 13 ran, 13 checks, 0 ignored, 0 filtered out, 0 ms)
```

## CppUTest Quick Reference
```cpp
// Integer comparison
LONGS_EQUAL(expected, actual);

// Floating point comparison (with tolerance)
DOUBLES_EQUAL(expected, actual, tolerance);

// Boolean checks
CHECK_TRUE(condition);
CHECK_FALSE(condition);
```

## Discussion Questions and Answers

### 1. Why do we use `DOUBLES_EQUAL` with a tolerance instead of exact comparison?

Floating point representation in binary cannot exactly represent most decimal values.
```cpp
float a = 0.1F + 0.2F;
float b = 0.3F;

// a == b ?
// Often: NO
```

Because `0.1` in binary is a repeating fraction—like `1/3` in decimal (0.333...). The computer rounds it.

**The fix:** Compare with tolerance:
```cpp
// Bad
CHECK_TRUE(result == 32.0F);

// Good
DOUBLES_EQUAL(32.0, result, 0.1);
```

The tolerance (0.1) says: "I expect 32.0, but accept 31.9 to 32.1."

**Choosing tolerance:**

| Situation | Typical Tolerance |
|-----------|-------------------|
| ADC conversion (12-bit) | 0.1 – 0.5 |
| Scientific calculation | 1e-6 – 1e-9 |
| Financial (use integers!) | Don't use float |
| Display rounding | 0.01 – 0.1 |

For the temperature sensor: 0.1°C tolerance is reasonable—it's within the noise floor of most sensors anyway.

---

### 2. What makes these functions easy to test?

They're **pure functions**:

| Property | Why It Helps Testing |
|----------|---------------------|
| No state | Same input → same output, every time |
| No side effects | Doesn't change anything outside itself |
| No dependencies | Doesn't need hardware, files, network, time |
| Deterministic | No randomness, no timing |

**Compare:**
```cpp
// Easy to test (pure)
float celsiusToFahrenheit(float celsius) {
    return (celsius * 9.0F / 5.0F) + 32.0F;
}

// Hard to test (impure)
float readTemperature() {
    uint16_t raw = ADC->DR;           // Hardware dependency
    logValue(raw);                     // Side effect
    lastReading = raw;                 // State change
    return convertToTemperature(raw);
}
```

**The pure function checklist:**

- ✅ All inputs come through parameters
- ✅ Output is only the return value
- ✅ No global variables read or written
- ✅ No hardware access
- ✅ No I/O (files, serial, network)
- ✅ No time/clock dependency

---

### 3. How would testing change if `adcToCelsius()` read directly from hardware?

You couldn't test it on your PC anymore.

**Current (testable):**
```cpp
float adcToCelsius(uint16_t adcValue) {
    const float normalized = static_cast<float>(adcValue)
                           / static_cast<float>(SensorConfig::ADC_MAX_VALUE);
    return (normalized * SensorConfig::TEMP_RANGE_CELSIUS)
           + SensorConfig::TEMP_MIN_CELSIUS;
}
```

Test runs anywhere—PC, CI server, colleague's laptop.

**Hypothetical (untestable):**
```cpp
float adcToCelsius() {
    uint16_t adcValue = ADC1->DR;  // Hardware register
    const float normalized = static_cast<float>(adcValue)
                           / static_cast<float>(SensorConfig::ADC_MAX_VALUE);
    return (normalized * SensorConfig::TEMP_RANGE_CELSIUS)
           + SensorConfig::TEMP_MIN_CELSIUS;
}
```

Now you need:

| Requirement | Problem |
|-------------|---------|
| Real hardware | Can't run on PC |
| Physical temperature | Heat gun? Freezer? |
| Precise control | How do you get exactly 20°C? |
| Repeatability | Room temperature varies |
| Speed | Flash, wait, observe, repeat |
| CI/CD | No hardware in build server |

**The lesson:** Keep hardware access at the edges. Push pure logic to the center.
```
┌─────────────────────────────────────────┐
│            Application Logic            │  ← Test this on PC
├─────────────────────────────────────────┤
│         Hardware Abstraction            │  ← Thin wrapper
├─────────────────────────────────────────┤
│              Hardware                   │  ← Test with JTAG, scope, HIL
└─────────────────────────────────────────┘
```

This is what Exercise 04 (mocking) will demonstrate—how to test the logic that *uses* a sensor without needing the actual sensor.

## Key Takeaways

1. **Pure functions are easy to test** — no setup, no teardown, no hardware
2. **Use tolerance for floating point** — binary can't represent all decimals exactly
3. **Separate logic from hardware** — testability is a design decision
4. **Test boundaries** — min, max, and just outside the valid range
5. **Use named constants** — `SensorConfig::TEMP_MIN_CELSIUS` not `-40.0F`
