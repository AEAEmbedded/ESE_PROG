#include "CppUTest/TestHarness.h"
#include "HardwareProxy.hpp"

using namespace hardware_proxy;

// ============================================================================
// HeartbeatIndicator Tests
// ============================================================================

TEST_GROUP(HeartbeatIndicator) {
    MockLed* led;
    HeartbeatIndicator* heartbeat;

    void setup() {
        led = new MockLed();
        heartbeat = new HeartbeatIndicator(*led);
    }

    void teardown() {
        delete heartbeat;
        delete led;
    }
};

TEST(HeartbeatIndicator, InitiallyLedIsOff) {
    CHECK_FALSE(led->isOn());
}

TEST(HeartbeatIndicator, FirstBeatTurnsLedOn) {
    heartbeat->beat();
    CHECK_TRUE(led->isOn());
}

TEST(HeartbeatIndicator, SecondBeatTurnsLedOff) {
    heartbeat->beat();  // on
    heartbeat->beat();  // off
    CHECK_FALSE(led->isOn());
}

TEST(HeartbeatIndicator, CountsBeatsCorrectly) {
    heartbeat->beat();
    heartbeat->beat();
    heartbeat->beat();
    LONGS_EQUAL(3, heartbeat->getBeatCount());
}

// ============================================================================
// ErrorIndicator Tests
// ============================================================================

TEST_GROUP(ErrorIndicator) {
    MockLed* led;
    ErrorIndicator* errorIndicator;

    void setup() {
        led = new MockLed();
        errorIndicator = new ErrorIndicator(*led);
    }

    void teardown() {
        delete errorIndicator;
        delete led;
    }
};

TEST(ErrorIndicator, ShowOkTurnsLedOff) {
    led->turnOn();  // Start with LED on
    errorIndicator->showOk();
    CHECK_FALSE(led->isOn());
}

TEST(ErrorIndicator, ShowErrorEndsWithLedOff) {
    errorIndicator->showError();
    CHECK_FALSE(led->isOn());
}

// ============================================================================
// Workshop Discussion Points
// ============================================================================

/**
 * KEY INSIGHT:
 *
 * Notice that HeartbeatIndicator and ErrorIndicator have NO #ifdef STM32,
 * NO hardware registers, NO HAL calls. They only know about ILed.
 *
 * This means:
 * 1. We can test them RIGHT NOW on our development PC
 * 2. We can run hundreds of tests in milliseconds
 * 3. We don't need to flash the target to verify behavior
 * 4. If we switch from STM32 to ESP32, these classes don't change!
 *
 * The ONLY place that knows about STM32 is Stm32Led.
 * That's the "humble object" - it's so simple it doesn't need testing.
 *
 * Clean Architecture: "Hardware is a detail"
 */
