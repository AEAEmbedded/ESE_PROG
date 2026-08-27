# Exercise 02: Testing a Class with Setup/Teardown

## Goal

Learn to test stateful classes using CppUTest's `TEST_GROUP` with `setup()` and `teardown()`.

## The Code Under Test

A `TemperatureFilter` class that smooths noisy sensor readings using a moving average.

| Method | Purpose |
|--------|---------|
| `addReading()` | Add a new temperature sample |
| `getFiltered()` | Get averaged temperature |
| `isReady()` | Check if window is full |
| `reset()` | Clear all samples |
| `getSampleCount()` | Current number of samples |

## The Problem with Stateful Classes

Unlike pure functions, classes have state. Each test might affect the next:
```cpp
// Danger: tests depend on each other
TemperatureFilter filter;

TEST(Filter, AddOne) {
    filter.addReading(20.0F);
    LONGS_EQUAL(1, filter.getSampleCount());  // Pass
}

TEST(Filter, AddAnother) {
    filter.addReading(25.0F);
    LONGS_EQUAL(1, filter.getSampleCount());  // FAIL! Count is 2
}
```

## The Solution: Setup/Teardown
```cpp
TEST_GROUP(Filter) {
    TemperatureFilter<4U>* filter;

    void setup() override {
        filter = new TemperatureFilter<4U>();  // Fresh for each test
    }

    void teardown() override {
        delete filter;  // Clean up
    }
};

TEST(Filter, AddOne) {
    filter->addReading(20.0F);
    LONGS_EQUAL(1, filter->getSampleCount());  // Pass
}

TEST(Filter, AddAnother) {
    filter->addReading(25.0F);
    LONGS_EQUAL(1, filter->getSampleCount());  // Pass! Fresh filter
}
```

## Build and Run
```bash
cmake -B build
cmake --build build
./build/test_temperature_filter
```

Expected output:
```
...............
OK (15 tests, 15 ran, 19 checks, 0 ignored, 0 filtered out, 0 ms)
```

## CppUTest Quick Reference
```cpp
TEST_GROUP(GroupName) {
    // Member variables available to all tests
    MyClass* obj;

    void setup() override {
        // Runs BEFORE each test
    }

    void teardown() override {
        // Runs AFTER each test
    }
};

TEST(GroupName, TestName) {
    // Test code here
}
```

## Discussion Questions

1. Why do we need a fresh object for each test?
2. What happens if you forget `teardown()`?
3. When would you pre-fill data in `setup()`?