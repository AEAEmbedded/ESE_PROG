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
    
    constexpr float EXPECTED_AVERAGE = 23.0F;  // (20 + 22 + 24 + 26) / 4
    DOUBLES_EQUAL(EXPECTED_AVERAGE, filter->getFiltered(), 0.01);
}

TEST(TemperatureFilterReadings, CountStopsAtWindowSize) {
    filter->addReading(20.0F);
    filter->addReading(21.0F);
    filter->addReading(22.0F);
    filter->addReading(23.0F);
    filter->addReading(24.0F);  // Overwrites oldest
    
    constexpr uint8_t WINDOW_SIZE = 4U;
    LONGS_EQUAL(WINDOW_SIZE, filter->getSampleCount());
}

TEST(TemperatureFilterReadings, AveragesPartialWindow) {
    filter->addReading(10.0F);
    filter->addReading(20.0F);
    
    constexpr float EXPECTED_AVERAGE = 15.0F;  // (10 + 20) / 2
    DOUBLES_EQUAL(EXPECTED_AVERAGE, filter->getFiltered(), 0.01);
}

// ============================================================================
// Circular Buffer Behavior
// ============================================================================

TEST_GROUP(TemperatureFilterCircular) {
    TemperatureFilter<4U>* filter;

    void setup() override {
        filter = new TemperatureFilter<4U>();
        filter->addReading(10.0F);
        filter->addReading(10.0F);
        filter->addReading(10.0F);
        filter->addReading(10.0F);
    }

    void teardown() override {
        delete filter;
    }
};

TEST(TemperatureFilterCircular, StartsWithCorrectAverage) {
    DOUBLES_EQUAL(10.0, filter->getFiltered(), 0.01);
}

TEST(TemperatureFilterCircular, NewReadingReplacesOldest) {
    filter->addReading(50.0F);
    
    constexpr float EXPECTED_AVERAGE = 20.0F;  // (50 + 10 + 10 + 10) / 4
    DOUBLES_EQUAL(EXPECTED_AVERAGE, filter->getFiltered(), 0.01);
}

TEST(TemperatureFilterCircular, TwoNewReadingsReplaceTwoOldest) {
    filter->addReading(50.0F);
    filter->addReading(50.0F);
    
    constexpr float EXPECTED_AVERAGE = 30.0F;  // (50 + 50 + 10 + 10) / 4
    DOUBLES_EQUAL(EXPECTED_AVERAGE, filter->getFiltered(), 0.01);
}

TEST(TemperatureFilterCircular, FullWindowRotation) {
    filter->addReading(30.0F);
    filter->addReading(30.0F);
    filter->addReading(30.0F);
    filter->addReading(30.0F);
    
    DOUBLES_EQUAL(30.0, filter->getFiltered(), 0.01);
}

TEST(TemperatureFilterCircular, StaysReadyAfterRotation) {
    filter->addReading(99.0F);
    filter->addReading(99.0F);
    
    CHECK_TRUE(filter->isReady());
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

TEST(TemperatureFilterReset, ReturnsZeroAfterReset) {
    filter->reset();
    
    DOUBLES_EQUAL(0.0, filter->getFiltered(), 0.01);
}

TEST(TemperatureFilterReset, CanAddReadingsAfterReset) {
    filter->reset();
    
    constexpr float NEW_READING = 99.0F;
    filter->addReading(NEW_READING);
    
    DOUBLES_EQUAL(NEW_READING, filter->getFiltered(), 0.01);
    LONGS_EQUAL(1U, filter->getSampleCount());
}

TEST(TemperatureFilterReset, FullFilterCanResetAndRefill) {
    filter->addReading(23.0F);  // Now full
    CHECK_TRUE(filter->isReady());
    
    filter->reset();
    CHECK_FALSE(filter->isReady());
    
    filter->addReading(50.0F);
    filter->addReading(50.0F);
    filter->addReading(50.0F);
    filter->addReading(50.0F);
    
    CHECK_TRUE(filter->isReady());
    DOUBLES_EQUAL(50.0, filter->getFiltered(), 0.01);
}

// ============================================================================
// Different Window Sizes
// ============================================================================

TEST_GROUP(TemperatureFilterWindowSize) {
};

TEST(TemperatureFilterWindowSize, WindowSizeTwo) {
    TemperatureFilter<2U> smallFilter;
    
    smallFilter.addReading(10.0F);
    smallFilter.addReading(20.0F);
    
    CHECK_TRUE(smallFilter.isReady());
    DOUBLES_EQUAL(15.0, smallFilter.getFiltered(), 0.01);
}

TEST(TemperatureFilterWindowSize, WindowSizeEight) {
    TemperatureFilter<8U> largeFilter;
    
    for (uint8_t i = 0U; i < 8U; ++i) {
        largeFilter.addReading(10.0F);
    }
    
    CHECK_TRUE(largeFilter.isReady());
    DOUBLES_EQUAL(10.0, largeFilter.getFiltered(), 0.01);
}

TEST(TemperatureFilterWindowSize, LargerWindowSmoothsMore) {
    TemperatureFilter<2U> smallFilter;
    TemperatureFilter<4U> largeFilter;
    
    // Add spike to both
    smallFilter.addReading(10.0F);
    smallFilter.addReading(100.0F);
    
    largeFilter.addReading(10.0F);
    largeFilter.addReading(10.0F);
    largeFilter.addReading(10.0F);
    largeFilter.addReading(100.0F);
    
    // Small window: (10 + 100) / 2 = 55
    DOUBLES_EQUAL(55.0, smallFilter.getFiltered(), 0.01);
    
    // Large window: (10 + 10 + 10 + 100) / 4 = 32.5
    DOUBLES_EQUAL(32.5, largeFilter.getFiltered(), 0.01);
}