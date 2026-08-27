# Exercise 02: Testing a Class with Setup/Teardown (Complete)

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

## File Structure
```
TemperatureTestComplex/
├── CMakeLists.txt
├── temperature_filter.hpp
├── test_temperature_filter.cpp
└── main.cpp
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

## Discussion Questions and Answers

### 1. Why do we need a fresh object for each test?

**Test isolation.** Each test must be independent—it should pass or fail regardless of which other tests run, or in what order.

Without isolation:

| Problem | Consequence |
|---------|-------------|
| Test order matters | Passes locally, fails in CI |
| One test pollutes another | Hard to debug—which test caused the failure? |
| Can't run single test | Must run all tests to get valid results |
| Flaky tests | Sometimes pass, sometimes fail |

**Example of pollution:**
```cpp
// Shared filter (bad)
TemperatureFilter filter;

TEST(Filter, FillWindow) {
    filter.addReading(20.0F);
    filter.addReading(21.0F);
    filter.addReading(22.0F);
    filter.addReading(23.0F);
    CHECK_TRUE(filter.isReady());  // Pass
}

TEST(Filter, StartsEmpty) {
    CHECK_FALSE(filter.isReady());  // FAIL! Still full from previous test
}
```

**With setup/teardown:**
```cpp
TEST_GROUP(Filter) {
    TemperatureFilter<4U>* filter;

    void setup() override {
        filter = new TemperatureFilter<4U>();
    }

    void teardown() override {
        delete filter;
    }
};

TEST(Filter, FillWindow) {
    filter->addReading(20.0F);
    filter->addReading(21.0F);
    filter->addReading(22.0F);
    filter->addReading(23.0F);
    CHECK_TRUE(filter->isReady());  // Pass
}

TEST(Filter, StartsEmpty) {
    CHECK_FALSE(filter->isReady());  // Pass! Fresh filter
}
```

---

### 2. What happens if you forget `teardown()`?

**Memory leak.** Each test allocates a new object in `setup()`, but nothing frees it.
```cpp
TEST_GROUP(Filter) {
    TemperatureFilter<4U>* filter;

    void setup() override {
        filter = new TemperatureFilter<4U>();  // Allocated
    }

    // No teardown—filter is never deleted!
};
```

**CppUTest has built-in memory leak detection.** With leak detection enabled, you'll see:
```
test_temperature_filter.cpp:42: error: Failure in TEST(Filter, StartsEmpty)
    Memory leak(s) found.
    Alloc num (4) Leak size: 20 Allocated at: test_temperature_filter.cpp:38
```

This is one of CppUTest's strengths for embedded development—it catches leaks that would eventually crash your target.

**Other consequences:**

| Effect | Impact |
|--------|--------|
| Memory grows each test | May run out of memory in large test suite |
| Resource exhaustion | File handles, sockets, hardware resources |
| Undefined behavior | Dangling pointers if object is reused |

---

### 3. When would you pre-fill data in `setup()`?

When multiple tests need the **same starting state** that's more complex than an empty object.

**Example: Testing circular buffer behavior**
```cpp
TEST_GROUP(TemperatureFilterCircular) {
    TemperatureFilter<4U>* filter;

    void setup() override {
        filter = new TemperatureFilter<4U>();
        // Pre-fill with known values
        filter->addReading(10.0F);
        filter->addReading(10.0F);
        filter->addReading(10.0F);
        filter->addReading(10.0F);
    }

    void teardown() override {
        delete filter;
    }
};

TEST(TemperatureFilterCircular, NewReadingReplacesOldest) {
    // Filter already contains [10, 10, 10, 10]
    filter->addReading(50.0F);
    DOUBLES_EQUAL(20.0, filter->getFiltered(), 0.01);
}

TEST(TemperatureFilterCircular, FullWindowRotation) {
    // Filter already contains [10, 10, 10, 10]
    filter->addReading(30.0F);
    filter->addReading(30.0F);
    filter->addReading(30.0F);
    filter->addReading(30.0F);
    DOUBLES_EQUAL(30.0, filter->getFiltered(), 0.01);
}
```

**Benefits of pre-filled setup:**

| Benefit | Example |
|---------|---------|
| Less repetition | Don't write 4x `addReading()` in every test |
| Clearer intent | Test focuses on what's being tested, not setup |
| Consistent starting point | All tests in group start from same state |
| Easier to read | "Given a full filter, when I add..." |

**When NOT to pre-fill:**

- Testing initial/empty state
- Testing the filling process itself
- When different tests need different starting states

Use multiple `TEST_GROUP`s for different starting states:
```cpp
TEST_GROUP(FilterEmpty) {
    // setup() creates empty filter
};

TEST_GROUP(FilterPartiallyFilled) {
    // setup() creates filter with 2 readings
};

TEST_GROUP(FilterFull) {
    // setup() creates filter with 4 readings
};
```

## Key Takeaways

1. **Test isolation is mandatory** — each test must be independent
2. **`setup()` runs before each test** — creates fresh state
3. **`teardown()` runs after each test** — cleans up resources
4. **Use multiple TEST_GROUPs** — for different starting states
5. **CppUTest detects memory leaks** — always pair `new` with `delete`
6. **Pre-fill when it clarifies** — reduces repetition, shows intent

## Connection to Embedded Development

Moving average filters are common in embedded systems:

| Application | Why |
|-------------|-----|
| Sensor smoothing | ADC noise reduction |
| Signal processing | Remove high-frequency noise |
| Control systems | Prevent actuator jitter |
| User input | Debounce analog inputs |

Testing the filter on PC means you can verify the algorithm works before deploying to hardware where debugging is harder.