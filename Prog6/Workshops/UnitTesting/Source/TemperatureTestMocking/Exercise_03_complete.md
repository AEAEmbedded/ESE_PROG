# Exercise 03: Dependency Injection + Mocking (Complete)

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

You can't—it requires real hardware, physical temperature control, and manual observation.

## The Solution: Dependency Injection
```cpp
class TemperatureController {
public:
    TemperatureController(ITemperatureSensor& sensor, IHeater& heater)
        : m_sensor{sensor}
        , m_heater{heater}
    {
    }

    void update() {
        float temp = m_sensor.read();      // Injected dependency
        if (temp < setpoint) {
            m_heater.turnOn();             // Injected dependency
        }
    }

private:
    ITemperatureSensor& m_sensor;
    IHeater& m_heater;
};
```

Now we can inject mock implementations for testing.

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

## Build and Run
```bash
cmake -B build
cmake --build build
./build/test_temperature_controller
```

Expected output:
```
......................
OK (22 tests, 22 ran, 28 checks, 0 ignored, 0 filtered out, 0 ms)
```

## The Mock Pattern

### 1. Define an Interface
```cpp
class ITemperatureSensor {
public:
    virtual ~ITemperatureSensor() = default;
    virtual float read() = 0;
    virtual bool isHealthy() const = 0;
};
```

### 2. Create a Mock Implementation
```cpp
class MockTemperatureSensor : public ITemperatureSensor {
public:
    float read() override {
        ++m_readCount;
        return m_temperature;
    }

    bool isHealthy() const override {
        return m_healthy;
    }

    // Test control
    void setTemperature(float temp) { m_temperature = temp; }
    void setHealthy(bool healthy) { m_healthy = healthy; }

    // Test inspection
    uint32_t getReadCount() const { return m_readCount; }

private:
    float m_temperature{20.0F};
    bool m_healthy{true};
    uint32_t m_readCount{0U};
};
```

### 3. Inject Mock in Tests
```cpp
TEST_GROUP(ControllerTest) {
    MockTemperatureSensor* sensor;
    MockHeater* heater;
    TemperatureController* controller;

    void setup() override {
        sensor = new MockTemperatureSensor();
        heater = new MockHeater();
        controller = new TemperatureController(*sensor, *heater);
    }

    void teardown() override {
        delete controller;
        delete heater;
        delete sensor;
    }
};

TEST(ControllerTest, TurnsHeaterOnWhenCold) {
    sensor->setTemperature(15.0F);
    
    controller->update();
    
    CHECK_TRUE(heater->isOn());
}
```

## Discussion Questions and Answers

### 1. Why do we use interfaces instead of concrete classes?

**Decoupling.** Interfaces let us swap implementations without changing the code that uses them.

| Without Interface | With Interface |
|-------------------|----------------|
| `TemperatureController` depends on `ADCSensor` | `TemperatureController` depends on `ITemperatureSensor` |
| Can only use one sensor type | Can use any sensor that implements interface |
| Can't test without hardware | Can inject mock for testing |
| Changing sensor requires changing controller | Changing sensor only requires new implementation |

**The Dependency Inversion Principle (SOLID):**

> High-level modules should not depend on low-level modules. Both should depend on abstractions.
```
Without DIP:                     With DIP:

┌──────────────────┐            ┌──────────────────┐
│    Controller    │            │    Controller    │
└────────┬─────────┘            └────────┬─────────┘
         │ depends on                    │ depends on
         ▼                               ▼
┌──────────────────┐            ┌──────────────────┐
│    ADCSensor     │            │ <<interface>>    │
│    (concrete)    │            │ ITemperatureSensor│
└──────────────────┘            └────────┬─────────┘
                                         │ implements
                          ┌──────────────┼──────────────┐
                          ▼              ▼              ▼
                    ┌──────────┐  ┌──────────┐  ┌──────────┐
                    │ADCSensor │  │I2CSensor │  │MockSensor│
                    └──────────┘  └──────────┘  └──────────┘
```

---

### 2. What's the difference between a mock and a stub?

Both are **test doubles**—fake objects used in place of real ones. The difference is in how you use them.

| Type | Purpose | Example |
|------|---------|---------|
| **Stub** | Provides canned answers | `sensor->setTemperature(25.0F)` — returns 25.0 when asked |
| **Mock** | Verifies interactions | `LONGS_EQUAL(1, sensor->getReadCount())` — checks it was called |

**Stub example:**
```cpp
// We only care about the OUTPUT (heater state)
sensor->setTemperature(15.0F);  // Stub returns 15.0
controller->update();
CHECK_TRUE(heater->isOn());     // Verify output
```

**Mock example:**
```cpp
// We care about the INTERACTION (was sensor read?)
sensor->resetReadCount();
controller->update();
LONGS_EQUAL(1U, sensor->getReadCount());  // Verify call happened
```

**Our `MockTemperatureSensor` is both:**

- Stub: `setTemperature()` controls what `read()` returns
- Mock: `getReadCount()` verifies how many times `read()` was called

**Martin Fowler's rule of thumb:**

> Stubs provide input to the system under test. Mocks verify output from the system under test.

---

### 3. When should you mock, and when should you use the real object?

**Mock when the dependency is:**

| Characteristic | Example | Why Mock |
|----------------|---------|----------|
| Hardware | ADC, GPIO, UART | Not available on PC |
| Slow | Database, network | Tests must be fast |
| Non-deterministic | Time, random | Tests must be repeatable |
| Has side effects | Email, payment | Don't want real effects |
| Hard to set up | Complex configuration | Mocking is simpler |
| Not yet implemented | Future module | Test your code now |

**Use the real object when it's:**

| Characteristic | Example | Why Real |
|----------------|---------|----------|
| Fast | Simple calculations | No benefit to mocking |
| Deterministic | Pure functions | Real behavior is predictable |
| Simple | Value objects | Mocking adds complexity |
| Part of the unit | Helper class | Test them together |

**Examples:**
```cpp
// MOCK: Hardware dependency
MockTemperatureSensor sensor;  // Can't use real ADC on PC

// MOCK: Slow dependency
MockDatabase db;  // Real DB would slow tests

// REAL: Simple value object
ControllerConfig config{20.0F, 1.0F};  // No reason to mock

// REAL: Pure function
float celsius = adcToCelsius(rawValue);  // Fast, deterministic
```

**Warning signs you're mocking too much:**

- Mocking classes you own that are simple
- Tests break when implementation changes (not behavior)
- Tests are longer than the code they test
- You're mocking the class under test

---

## Key Takeaways

1. **Interfaces enable testing** — depend on abstractions, not hardware
2. **Dependency injection** — pass dependencies via constructor
3. **Mocks control input** — `setTemperature()`, `setHealthy()`
4. **Mocks verify behavior** — `getReadCount()`, `getTurnOnCount()`
5. **Test the logic, not the framework** — don't mock everything
6. **Same code, different environments** — mocks in test, real hardware in production

## Connection to Embedded Development

This pattern is essential for embedded systems:

| Production | Test |
|------------|------|
| `AdcTemperatureSensor` | `MockTemperatureSensor` |
| `GpioHeater` | `MockHeater` |
| Runs on STM32 | Runs on PC |
| Milliseconds per control loop | Thousands of tests per second |
| Debug with JTAG | Debug with IDE |

The business logic (`TemperatureController`) is identical in both environments. Only the dependencies change.
```
┌─────────────────────────────────────────────────────────────┐
│                  TemperatureController                       │
│                    (tested on PC)                           │
└─────────────────────────┬───────────────────────────────────┘
                          │
            ┌─────────────┴─────────────┐
            │                           │
    ┌───────▼───────┐           ┌───────▼───────┐
    │  Production   │           │     Test      │
    ├───────────────┤           ├───────────────┤
    │ AdcSensor     │           │ MockSensor    │
    │ GpioHeater    │           │ MockHeater    │
    │ Runs on MCU   │           │ Runs on PC    │
    └───────────────┘           └───────────────┘
```