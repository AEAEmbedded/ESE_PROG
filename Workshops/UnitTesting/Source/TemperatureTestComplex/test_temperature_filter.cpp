#include "CppUTest/TestHarness.h"
#include "temperature_filter.hpp"

using namespace temperature;

// ============================================================================
// Filter Initialization
// ============================================================================

TEST_GROUP(TemperatureFilterInit) {
    TemperatureFilter<4U>* filter;

    void setup() override {
        filter = new TemperatureFilter<4U>();
    }

    void teardown() override {
        delete filter;
    }
};

TEST(TemperatureFilterInit, StartsEmpty) {
    LONGS_EQUAL(0U, filter->getSampleCount());
}

TEST(TemperatureFilterInit, NotReadyWhenEmpty) {
    CHECK_FALSE(filter->isReady());
}

TEST(TemperatureFilterInit, EmptyFilterReturnsZero) {
    DOUBLES_EQUAL(0.0, filter->getFiltered(), 0.01);
}

// ============================================================================
// Adding Readings
// ============================================================================

TEST_GROUP(TemperatureFilterReadings) {
    TemperatureFilter<4U>* filter;

    void setup() override {
        filter = new TemperatureFilter<4U>();
    }

    void teardown() override {
        delete filter;
    }
};

TEST(TemperatureFilterReadings, SingleReadingReturnsItself) {
    constexpr float READING = 25.0F;
    
    filter->addReading(READING);
    
    DOUBLES_EQUAL(READING, filter->getFiltered(), 0.01);
}

TEST(TemperatureFilterReadings, CountIncrementsWithEachReading) {
    filter->addReading(20.0F);
    LONGS_EQUAL(1U, filter->getSampleCount());
    
    filter->addReading(21.0F);
    LONGS_EQUAL(2U, filter->getSampleCount());
    
    filter->addReading(22.0F);
    LONGS_EQUAL(3U, filter->getSampleCount());
}

TEST(TemperatureFilterReadings, ReadyWhenWindowFull) {
    filter->addReading(20.0F);
    filter->addReading(21.0F);
    filter->addReading(22.0F);
    CHECK_FALSE(filter->isReady());
    
    filter->addReading(23.0F);
    CHECK_TRUE(filter->isReady());
}

TEST(TemperatureFilterReadings, AveragesMultipleReadings) {
    filter->addReading(20.0F);
    filter->addReading(22.0F);
    filter->addReading(24.0F);
    filter->addReading(26.0F);
    
    // Average: (20 + 22 + 24 + 26) / 4 = 23
    DOUBLES_EQUAL(23.0, filter->getFiltered(), 0.01);
}

TEST(TemperatureFilterReadings, CountStopsAtWindowSize) {
    filter->addReading(20.0F);
    filter->addReading(21.0F);
    filter->addReading(22.0F);
    filter->addReading(23.0F);
    filter->addReading(24.0F);  // Overwrites oldest
    
    LONGS_EQUAL(4U, filter->getSampleCount());
}

// ============================================================================
// Circular Buffer Behavior
// ============================================================================

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
    // Filter contains [10, 10, 10, 10], average = 10
    DOUBLES_EQUAL(10.0, filter->getFiltered(), 0.01);
    
    // Add 50, replaces first 10: [50, 10, 10, 10]
    filter->addReading(50.0F);
    
    // Average: (50 + 10 + 10 + 10) / 4 = 20
    DOUBLES_EQUAL(20.0, filter->getFiltered(), 0.01);
}

TEST(TemperatureFilterCircular, FullWindowRotation) {
    // Replace all values with 30
    filter->addReading(30.0F);
    filter->addReading(30.0F);
    filter->addReading(30.0F);
    filter->addReading(30.0F);
    
    DOUBLES_EQUAL(30.0, filter->getFiltered(), 0.01);
}

// ============================================================================
// Reset
// ============================================================================

TEST_GROUP(TemperatureFilterReset) {
    TemperatureFilter<4U>* filter;

    void setup() override {
        filter = new TemperatureFilter<4U>();
        filter->addReading(20.0F);
        filter->addReading(21.0F);
        filter->addReading(22.0F);
    }

    void teardown() override {
        delete filter;
    }
};

TEST(TemperatureFilterReset, ClearsSampleCount) {
    filter->reset();
    
    LONGS_EQUAL(0U, filter->getSampleCount());
}

TEST(TemperatureFilterReset, NotReadyAfterReset) {
    filter->reset();
    
    CHECK_FALSE(filter->isReady());
}

TEST(TemperatureFilterReset, CanAddReadingsAfterReset) {
    filter->reset();
    
    constexpr float NEW_READING = 99.0F;
    filter->addReading(NEW_READING);
    
    DOUBLES_EQUAL(NEW_READING, filter->getFiltered(), 0.01);
    LONGS_EQUAL(1U, filter->getSampleCount());
}