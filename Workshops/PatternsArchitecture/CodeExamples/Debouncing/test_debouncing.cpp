#include "CppUTest/TestHarness.h"
#include "Debouncing.hpp"

using namespace debouncing;

// ============================================================================
// DelayDebouncer Tests
// ============================================================================

TEST_GROUP(DelayDebouncer) {
    MockButton* button;
    DelayDebouncer* debouncer;

    void setup() {
        button = new MockButton();
        debouncer = new DelayDebouncer(*button, 10);  // 10ms debounce
    }

    void teardown() {
        delete debouncer;
        delete button;
    }

    void updateNTimes(int n) {
        for (int i = 0; i < n; i++) {
            debouncer->update();
        }
    }
};

TEST(DelayDebouncer, StartsNotPressed) {
    CHECK_FALSE(debouncer->isPressed());
}

TEST(DelayDebouncer, DoesNotChangeDuringDebounce) {
    button->press();
    updateNTimes(5);  // Only 5ms, need 10ms

    CHECK_FALSE(debouncer->isPressed());  // Not yet!
}

TEST(DelayDebouncer, ChangesAfterDebounceTime) {
    button->press();
    updateNTimes(10);  // Full 10ms

    CHECK_TRUE(debouncer->isPressed());
    CHECK_TRUE(debouncer->stateChanged());
}

TEST(DelayDebouncer, RejectsBounce) {
    // Simulate bouncy press
    button->press();
    updateNTimes(3);
    button->release();  // Bounce!
    updateNTimes(2);
    button->press();    // Bounce back!
    updateNTimes(10);   // Now stable

    CHECK_TRUE(debouncer->isPressed());
}

TEST(DelayDebouncer, DetectsRelease) {
    // First press
    button->press();
    updateNTimes(10);
    CHECK_TRUE(debouncer->isPressed());

    // Then release
    button->release();
    updateNTimes(10);
    CHECK_FALSE(debouncer->isPressed());
}

// ============================================================================
// ShiftRegisterDebouncer Tests
// ============================================================================

TEST_GROUP(ShiftRegisterDebouncer) {
    MockButton* button;
    ShiftRegisterDebouncer* debouncer;

    void setup() {
        button = new MockButton();
        debouncer = new ShiftRegisterDebouncer(*button);
    }

    void teardown() {
        delete debouncer;
        delete button;
    }

    void updateNTimes(int n) {
        for (int i = 0; i < n; i++) {
            debouncer->update();
        }
    }
};

TEST(ShiftRegisterDebouncer, StartsNotPressed) {
    CHECK_FALSE(debouncer->isPressed());
}

TEST(ShiftRegisterDebouncer, NeedsEightConsecutiveReadings) {
    button->press();
    updateNTimes(7);
    CHECK_FALSE(debouncer->isPressed());  // Not yet!

    debouncer->update();  // 8th reading
    CHECK_TRUE(debouncer->isPressed());
}

TEST(ShiftRegisterDebouncer, HistoryShowsBitPattern) {
    button->press();
    updateNTimes(4);

    // History should be 0b00001111 = 0x0F
    LONGS_EQUAL(0x0F, debouncer->getHistory());
}

TEST(ShiftRegisterDebouncer, BounceResetsHistory) {
    button->press();
    updateNTimes(6);
    button->release();  // Bounce!
    debouncer->update();

    // History now has a 0 in it, not all 1s
    CHECK_FALSE(debouncer->isPressed());
}

// ============================================================================
// IntegratorDebouncer Tests
// ============================================================================

TEST_GROUP(IntegratorDebouncer) {
    MockButton* button;
    IntegratorDebouncer* debouncer;

    void setup() {
        button = new MockButton();
        debouncer = new IntegratorDebouncer(*button, 5);  // maxCount = 5
    }

    void teardown() {
        delete debouncer;
        delete button;
    }

    void updateNTimes(int n) {
        for (int i = 0; i < n; i++) {
            debouncer->update();
        }
    }
};

TEST(IntegratorDebouncer, StartsNotPressed) {
    CHECK_FALSE(debouncer->isPressed());
    LONGS_EQUAL(0, debouncer->getCounter());
}

TEST(IntegratorDebouncer, CounterIncrementsOnPress) {
    button->press();
    updateNTimes(3);

    LONGS_EQUAL(3, debouncer->getCounter());
    CHECK_FALSE(debouncer->isPressed());  // Not at max yet
}

TEST(IntegratorDebouncer, ChangesAtMaxCount) {
    button->press();
    updateNTimes(5);

    LONGS_EQUAL(5, debouncer->getCounter());
    CHECK_TRUE(debouncer->isPressed());
}

TEST(IntegratorDebouncer, CounterDecrementsOnRelease) {
    button->press();
    updateNTimes(5);  // Now pressed, counter = 5

    button->release();
    updateNTimes(2);

    LONGS_EQUAL(3, debouncer->getCounter());
    CHECK_TRUE(debouncer->isPressed());  // Still pressed
}

TEST(IntegratorDebouncer, ReleasesAtZero) {
    button->press();
    updateNTimes(5);

    button->release();
    updateNTimes(5);

    LONGS_EQUAL(0, debouncer->getCounter());
    CHECK_FALSE(debouncer->isPressed());
}

TEST(IntegratorDebouncer, HandlesBounceGracefully) {
    // Press with bounces
    button->press();
    updateNTimes(3);   // counter = 3
    button->release(); // Bounce
    debouncer->update();  // counter = 2
    button->press();
    updateNTimes(4);   // counter = 5 (capped at max)

    CHECK_TRUE(debouncer->isPressed());
}

// ============================================================================
// Workshop Discussion
// ============================================================================

/**
 * DEBOUNCING PATTERN COMPARISON:
 *
 * Delay-Based:
 *   + Simple to understand and implement
 *   + Predictable timing
 *   - Adds fixed latency to response
 *   - Needs a timer/counter
 *
 * Shift Register:
 *   + Very efficient (just bit shifts)
 *   + Minimal memory (1 byte history)
 *   + Configurable depth (use uint16_t for 16 samples)
 *   - Requires consistent polling rate
 *
 * Integrator:
 *   + Handles noisy signals well
 *   + Smooth behavior with lots of bouncing
 *   + Separate thresholds for press/release possible
 *   - More complex logic
 *   - More memory (counter variable)
 *
 * TYPICAL VALUES:
 *   - Debounce time: 10-50ms (depends on switch quality)
 *   - Polling rate: 1-5ms
 *   - Shift register: 8-16 samples
 *   - Integrator max: 5-20 counts
 *
 * HARDWARE ALTERNATIVES:
 *   - RC filter on switch input (simple, passive)
 *   - Schmitt trigger IC (e.g., 74HC14)
 *   - Dedicated debounce IC
 */
