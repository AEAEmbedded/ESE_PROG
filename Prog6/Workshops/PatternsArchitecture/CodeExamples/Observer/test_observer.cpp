#include "CppUTest/TestHarness.h"
#include "Observer.hpp"

using namespace observer;

// ============================================================================
// ButtonSubject Tests
// ============================================================================

TEST_GROUP(ButtonSubject) {
    ButtonSubject<4>* subject;
    LedController* ledController;
    EventLogger* logger;

    void setup() {
        subject = new ButtonSubject<4>();
        ledController = new LedController();
        logger = new EventLogger();
    }

    void teardown() {
        delete logger;
        delete ledController;
        delete subject;
    }
};

TEST(ButtonSubject, CanAttachObserver) {
    CHECK_TRUE(subject->attach(ledController));
    LONGS_EQUAL(1, subject->getObserverCount());
}

TEST(ButtonSubject, CanAttachMultipleObservers) {
    subject->attach(ledController);
    subject->attach(logger);
    LONGS_EQUAL(2, subject->getObserverCount());
}

TEST(ButtonSubject, NotifiesAllObservers) {
    subject->attach(ledController);
    subject->attach(logger);

    subject->notifyPressed(1);

    CHECK_TRUE(ledController->isLedOn());
    LONGS_EQUAL(1, logger->getLogCount());
}

TEST(ButtonSubject, CanDetachObserver) {
    subject->attach(ledController);
    subject->attach(logger);

    subject->detach(ledController);

    LONGS_EQUAL(1, subject->getObserverCount());
}

TEST(ButtonSubject, DetachedObserverNotNotified) {
    subject->attach(ledController);
    subject->detach(ledController);

    subject->notifyPressed(1);

    CHECK_FALSE(ledController->isLedOn());  // Not notified
}

TEST(ButtonSubject, RejectsWhenFull) {
    ButtonSubject<2> smallSubject;
    LedController led1, led2, led3;

    CHECK_TRUE(smallSubject.attach(&led1));
    CHECK_TRUE(smallSubject.attach(&led2));
    CHECK_FALSE(smallSubject.attach(&led3));  // No room!
}

// ============================================================================
// LedController Tests
// ============================================================================

TEST_GROUP(LedController) {
    LedController* led;

    void setup() { led = new LedController(); }
    void teardown() { delete led; }
};

TEST(LedController, StartsOff) {
    CHECK_FALSE(led->isLedOn());
}

TEST(LedController, TogglesOnPress) {
    led->onButtonPressed(1);
    CHECK_TRUE(led->isLedOn());

    led->onButtonPressed(1);
    CHECK_FALSE(led->isLedOn());
}

TEST(LedController, CountsPresses) {
    led->onButtonPressed(1);
    led->onButtonPressed(1);
    led->onButtonPressed(1);
    LONGS_EQUAL(3, led->getPressCount());
}

// ============================================================================
// TemperatureSubject Tests
// ============================================================================

TEST_GROUP(TemperatureSubject) {
    TemperatureSubject<4>* sensor;
    TemperatureDisplay* display;
    SafetyController* safety;

    void setup() {
        sensor = new TemperatureSubject<4>(50.0f);  // 50C threshold
        display = new TemperatureDisplay();
        safety = new SafetyController();
    }

    void teardown() {
        delete safety;
        delete display;
        delete sensor;
    }
};

TEST(TemperatureSubject, DisplayUpdatesOnChange) {
    sensor->attach(display);

    sensor->updateTemperature(25.5f);

    DOUBLES_EQUAL(25.5f, display->getDisplayValue(), 0.01);
}

TEST(TemperatureSubject, NoAlarmBelowThreshold) {
    sensor->attach(display);
    sensor->attach(safety);

    sensor->updateTemperature(45.0f);

    CHECK_FALSE(display->isAlarmActive());
    CHECK_FALSE(safety->isShutdownTriggered());
}

TEST(TemperatureSubject, AlarmAboveThreshold) {
    sensor->attach(display);
    sensor->attach(safety);

    sensor->updateTemperature(55.0f);  // Above 50C threshold

    CHECK_TRUE(display->isAlarmActive());
    CHECK_TRUE(safety->isShutdownTriggered());
}

TEST(TemperatureSubject, MultipleObserversAllNotified) {
    TemperatureDisplay display2;
    sensor->attach(display);
    sensor->attach(&display2);

    sensor->updateTemperature(30.0f);

    DOUBLES_EQUAL(30.0f, display->getDisplayValue(), 0.01);
    DOUBLES_EQUAL(30.0f, display2.getDisplayValue(), 0.01);
}

// ============================================================================
// Workshop Discussion
// ============================================================================

/**
 * OBSERVER PATTERN IN EMBEDDED SYSTEMS:
 *
 * Use cases:
 * - Button/switch input handling
 * - Sensor data distribution
 * - ISR to application communication
 * - Event-driven architectures
 *
 * Embedded-specific considerations:
 *
 * 1. FIXED-SIZE OBSERVER LIST
 *    No malloc/new in ISR context. Pre-allocate maximum observers.
 *
 * 2. ISR SAFETY
 *    Keep notify() short! Consider:
 *    - Set a flag in ISR, process in main loop
 *    - Use a queue for deferred processing
 *    - Disable interrupts briefly if modifying observer list
 *
 * 3. AVOID CIRCULAR NOTIFICATIONS
 *    Observer A notifies Subject B notifies Observer A...
 *    Use flags or state to prevent re-entry.
 *
 * 4. MEMORY
 *    Each observer is just a pointer (4-8 bytes).
 *    The pattern itself has minimal overhead.
 *
 * Related patterns:
 * - Publish-Subscribe (more decoupled, with message broker)
 * - Mediator (centralized communication)
 * - Event Queue (deferred processing)
 */
