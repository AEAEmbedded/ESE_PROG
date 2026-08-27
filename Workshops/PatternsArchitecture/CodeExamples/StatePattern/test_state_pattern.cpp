#include "CppUTest/TestHarness.h"
#include "StatePattern.hpp"

using namespace state_pattern;

// ============================================================================
// Tests for Switch-Case Implementation
// ============================================================================

TEST_GROUP(HeaterSwitchCase) {
    HeaterSwitchCase* heater;

    void setup() { heater = new HeaterSwitchCase(); }
    void teardown() { delete heater; }
};

TEST(HeaterSwitchCase, StartsInOffState) {
    CHECK_EQUAL(HeaterState::OFF, heater->getState());
    CHECK_FALSE(heater->isHeaterOn());
}

TEST(HeaterSwitchCase, TurnOnStartsHeating) {
    heater->handleEvent(HeaterEvent::TURN_ON);
    CHECK_EQUAL(HeaterState::HEATING, heater->getState());
    CHECK_TRUE(heater->isHeaterOn());
}

TEST(HeaterSwitchCase, TempOkStopsHeating) {
    heater->handleEvent(HeaterEvent::TURN_ON);
    heater->handleEvent(HeaterEvent::TEMP_OK);
    CHECK_EQUAL(HeaterState::TARGET_REACHED, heater->getState());
    CHECK_FALSE(heater->isHeaterOn());
}

TEST(HeaterSwitchCase, TempLowRestartsHeating) {
    heater->handleEvent(HeaterEvent::TURN_ON);
    heater->handleEvent(HeaterEvent::TEMP_OK);
    heater->handleEvent(HeaterEvent::TEMP_LOW);
    CHECK_EQUAL(HeaterState::HEATING, heater->getState());
    CHECK_TRUE(heater->isHeaterOn());
}

// ============================================================================
// Tests for State Table Implementation
// ============================================================================

TEST_GROUP(HeaterStateTable) {
    HeaterStateTable* heater;

    void setup() { heater = new HeaterStateTable(); }
    void teardown() { delete heater; }
};

TEST(HeaterStateTable, StartsInOffState) {
    CHECK_EQUAL(HeaterState::OFF, heater->getState());
}

TEST(HeaterStateTable, TurnOnStartsHeating) {
    heater->handleEvent(HeaterEvent::TURN_ON);
    CHECK_EQUAL(HeaterState::HEATING, heater->getState());
    CHECK_TRUE(heater->isHeaterOn());
}

TEST(HeaterStateTable, FullCycleWorks) {
    heater->handleEvent(HeaterEvent::TURN_ON);
    heater->handleEvent(HeaterEvent::TEMP_OK);
    heater->handleEvent(HeaterEvent::TEMP_LOW);
    heater->handleEvent(HeaterEvent::TURN_OFF);
    CHECK_EQUAL(HeaterState::OFF, heater->getState());
}

// ============================================================================
// Tests for State Pattern Implementation
// ============================================================================

TEST_GROUP(HeaterStatePattern) {
    HeaterContext* heater;

    void setup() { heater = new HeaterContext(); }
    void teardown() { delete heater; }
};

TEST(HeaterStatePattern, StartsInOffState) {
    CHECK_EQUAL(HeaterState::OFF, heater->getState());
    CHECK_FALSE(heater->isHeaterOn());
}

TEST(HeaterStatePattern, TurnOnStartsHeating) {
    heater->handleEvent(HeaterEvent::TURN_ON);
    CHECK_EQUAL(HeaterState::HEATING, heater->getState());
    CHECK_TRUE(heater->isHeaterOn());
}

TEST(HeaterStatePattern, OnEntryActionActivatesHeater) {
    CHECK_FALSE(heater->isHeaterOn());
    heater->handleEvent(HeaterEvent::TURN_ON);
    // The HeatingState::onEnter() should have activated the heater
    CHECK_TRUE(heater->isHeaterOn());
}

TEST(HeaterStatePattern, OnExitActionDeactivatesHeater) {
    heater->handleEvent(HeaterEvent::TURN_ON);
    CHECK_TRUE(heater->isHeaterOn());
    heater->handleEvent(HeaterEvent::TEMP_OK);
    // The HeatingState::onExit() should have deactivated the heater
    CHECK_FALSE(heater->isHeaterOn());
}

TEST(HeaterStatePattern, IgnoresInvalidEvents) {
    // TEMP_OK in OFF state should be ignored
    heater->handleEvent(HeaterEvent::TEMP_OK);
    CHECK_EQUAL(HeaterState::OFF, heater->getState());
}

// ============================================================================
// Comparison: All Three Behave the Same!
// ============================================================================

TEST_GROUP(AllImplementationsMatch) {
    HeaterSwitchCase* switchCase;
    HeaterStateTable* stateTable;
    HeaterContext* statePattern;

    void setup() {
        switchCase = new HeaterSwitchCase();
        stateTable = new HeaterStateTable();
        statePattern = new HeaterContext();
    }

    void teardown() {
        delete switchCase;
        delete stateTable;
        delete statePattern;
    }
};

TEST(AllImplementationsMatch, SameResultsForFullCycle) {
    // Apply same sequence to all three
    HeaterEvent sequence[] = {
        HeaterEvent::TURN_ON,
        HeaterEvent::TEMP_OK,
        HeaterEvent::TEMP_LOW,
        HeaterEvent::TEMP_OK,
        HeaterEvent::TURN_OFF
    };

    for (auto event : sequence) {
        switchCase->handleEvent(event);
        stateTable->handleEvent(event);
        statePattern->handleEvent(event);
    }

    // All should end up in same state
    CHECK_EQUAL(switchCase->getState(), stateTable->getState());
    CHECK_EQUAL(stateTable->getState(), statePattern->getState());
    CHECK_EQUAL(HeaterState::OFF, statePattern->getState());
}

// ============================================================================
// Workshop Discussion
// ============================================================================

/**
 * WHEN TO USE WHICH:
 *
 * Switch-Case:
 *   - Very simple FSMs (2-3 states, 2-3 events)
 *   - Throwaway/prototype code
 *   - WARNING: Becomes unmaintainable as complexity grows!
 *
 * State Table:
 *   - Medium complexity FSMs
 *   - When you want to visualize/generate from UML
 *   - When transitions are regular (same action types)
 *   - Good for code review (table is easy to verify)
 *
 * State Pattern:
 *   - Complex FSMs with many states
 *   - When states have different behaviors (not just transitions)
 *   - When you need entry/exit actions
 *   - When you want to add states without modifying existing code (OCP)
 *   - When individual states need unit testing
 *
 * See also: Hierarchical State Machines (HSM), AND-states (parallel regions)
 */
