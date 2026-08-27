# Exercise 03: Dependency Injection + Mocking

## Goal

Learn to test code that depends on hardware by using interfaces and mock objects.

## The Problem

How do you test this?
```cpp
class TemperatureController {
public:
    void update() {
        float temp = ADC1->DR * 0.03F - 40.0F;  // Read hardware directly
        if (temp < setpoint) {
            GPIOA->ODR |= HEATER_PIN;           // Control hardware directly
        }
    }
};
```

You can't—it requires real hardware.

## The Solution: Dependency Injection

1. Define **interfaces** for hardware dependencies
2. **Inject** those interfaces via constructor
3. In production: pass real hardware implementations
4. In tests: pass **mock** implementations

## The Code Under Test

A `TemperatureController` that:

- Reads temperature from a sensor
- Controls a heater to maintain setpoint
- Uses hysteresis to prevent rapid switching
- Enters fault state if sensor fails

| Class | Purpose |
|-------|---------|
| `ITemperatureSensor` | Interface for temperature reading |
| `IHeater` | Interface for heater control |
| `TemperatureController` | The logic we want to test |
| `MockTemperatureSensor` | Fake sensor for testing |
| `MockHeater` | Fake heater for testing |

## File Structure
```
TemperatureTestMocking/
├── CMakeLists.txt
├── i_temperature_sensor.hpp
├── i_heater.hpp
├── temperature_controller.hpp
├── temperature_controller.cpp
├── mock_temperature_sensor.hpp
├── mock_heater.hpp
├── test_temperature_controller.cpp
└── main.cpp
```

## Your Task

Complete the TODO tests in `test_temperature_controller.cpp`.

### Part 1: Normal Operation
```cpp
// TODO: Students implement
// TEST(TemperatureControllerNormal, TurnsHeaterOnWhenTooCold)
// TEST(TemperatureControllerNormal, TurnsHeaterOffWhenTooHot)
```

Use `sensor->setTemperature()` to simulate different temperatures.

### Part 2: Fault Handling
```cpp
// TODO: Students implement
// TEST(TemperatureControllerFault, EntersFaultWhenSensorUnhealthy)
// TEST(TemperatureControllerFault, TurnsHeaterOffOnFault)
```

Use `sensor->setHealthy(false)` to simulate sensor failure.

### Part 3: Hysteresis
```cpp
// TODO: Students implement
// TEST(TemperatureControllerHysteresis, PreventsRapidSwitching)
```

Verify the heater doesn't toggle when temperature is within the hysteresis band.

## Build and Run
```bash
cmake -B build
cmake --build build
./build/test_temperature_controller
```

## CppUTest Quick Reference
```cpp
// Boolean checks
CHECK_TRUE(heater->isOn());
CHECK_FALSE(controller->isInFault());

// Value comparison
DOUBLES_EQUAL(20.0, controller->getSetpoint(), 0.01);
LONGS_EQUAL(1U, sensor->getReadCount());
```

## Mock Pattern
```cpp
// 1. Set up the mock's behavior
sensor->setTemperature(15.0F);
sensor->setHealthy(true);

// 2. Call the code under test
controller->update();

// 3. Check the results
CHECK_TRUE(heater->isOn());

// 4. Optionally verify interactions
LONGS_EQUAL(1U, sensor->getReadCount());
```

## Discussion Questions

1. Why do we use interfaces instead of concrete classes?
2. What's the difference between a mock and a stub?
3. When should you mock, and when should you use the real object?