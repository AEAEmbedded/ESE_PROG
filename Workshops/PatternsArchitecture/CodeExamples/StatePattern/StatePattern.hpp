#ifndef STATE_PATTERN_HPP
#define STATE_PATTERN_HPP

#include <cstdint>
#include <functional>

/**
 * =============================================================================
 * STATE MACHINE PATTERNS FOR EMBEDDED SYSTEMS
 * =============================================================================
 *
 * This file demonstrates THREE approaches to state machines:
 *
 * 1. SWITCH-CASE (the "before" - often becomes unmaintainable)
 * 2. STATE TABLE (table-driven, compact, good for simple FSMs)
 * 3. STATE PATTERN (OO approach, extensible, testable)
 *
 * Example: A simple heater controller
 *   States: OFF, HEATING, TARGET_REACHED
 *   Events: TURN_ON, TURN_OFF, TEMP_LOW, TEMP_OK
 *
 * Douglass (Ch.5):
 *   "State machines are the most common behavioral pattern in embedded systems"
 *
 * =============================================================================
 */

namespace state_pattern {

// ============================================================================
// Common Types
// ============================================================================

enum class HeaterState {
    OFF,
    HEATING,
    TARGET_REACHED
};

enum class HeaterEvent {
    TURN_ON,
    TURN_OFF,
    TEMP_LOW,
    TEMP_OK
};

// ============================================================================
// APPROACH 1: Switch-Case (the "BEFORE" - anti-pattern when it grows)
// ============================================================================

/**
 * @brief Heater controller using switch-case
 *
 * Problems:
 * - Adding states/events requires modifying multiple places
 * - Easy to forget a case
 * - Hard to test individual state behaviors
 * - Becomes a "god function" as complexity grows
 */
class HeaterSwitchCase {
public:
    HeaterSwitchCase() : state_(HeaterState::OFF), heaterOn_(false) {}

    void handleEvent(HeaterEvent event) {
        switch (state_) {
            case HeaterState::OFF:
                switch (event) {
                    case HeaterEvent::TURN_ON:
                        heaterOn_ = true;
                        state_ = HeaterState::HEATING;
                        break;
                    case HeaterEvent::TURN_OFF:
                        // Already off, ignore
                        break;
                    case HeaterEvent::TEMP_LOW:
                    case HeaterEvent::TEMP_OK:
                        // Ignore temperature events when off
                        break;
                }
                break;

            case HeaterState::HEATING:
                switch (event) {
                    case HeaterEvent::TURN_OFF:
                        heaterOn_ = false;
                        state_ = HeaterState::OFF;
                        break;
                    case HeaterEvent::TEMP_OK:
                        heaterOn_ = false;
                        state_ = HeaterState::TARGET_REACHED;
                        break;
                    case HeaterEvent::TURN_ON:
                    case HeaterEvent::TEMP_LOW:
                        // Already heating, ignore
                        break;
                }
                break;

            case HeaterState::TARGET_REACHED:
                switch (event) {
                    case HeaterEvent::TURN_OFF:
                        state_ = HeaterState::OFF;
                        break;
                    case HeaterEvent::TEMP_LOW:
                        heaterOn_ = true;
                        state_ = HeaterState::HEATING;
                        break;
                    case HeaterEvent::TURN_ON:
                    case HeaterEvent::TEMP_OK:
                        // Ignore
                        break;
                }
                break;
        }
    }

    HeaterState getState() const { return state_; }
    bool isHeaterOn() const { return heaterOn_; }

private:
    HeaterState state_;
    bool heaterOn_;
};

// ============================================================================
// APPROACH 2: State Table (table-driven)
// ============================================================================

/**
 * @brief Heater controller using state table
 *
 * Advantages:
 * - Compact representation
 * - Easy to visualize as a table
 * - Good for code generation from UML
 *
 * Disadvantages:
 * - Actions are harder to express (need function pointers or lambdas)
 * - Less flexible for complex entry/exit actions
 */
class HeaterStateTable {
public:
    HeaterStateTable() : state_(HeaterState::OFF), heaterOn_(false) {}

    void handleEvent(HeaterEvent event) {
        // Find transition in table
        for (const auto& transition : transitions_) {
            if (transition.currentState == state_ && transition.event == event) {
                // Execute action
                if (transition.action) {
                    transition.action(*this);
                }
                // Transition to new state
                state_ = transition.nextState;
                return;
            }
        }
        // No transition found - event ignored in this state
    }

    HeaterState getState() const { return state_; }
    bool isHeaterOn() const { return heaterOn_; }

    // Actions (called by table)
    void turnHeaterOn() { heaterOn_ = true; }
    void turnHeaterOff() { heaterOn_ = false; }

private:
    struct Transition {
        HeaterState currentState;
        HeaterEvent event;
        HeaterState nextState;
        std::function<void(HeaterStateTable&)> action;
    };

    // The state table - defines all valid transitions
    static constexpr size_t NUM_TRANSITIONS = 5;
    const Transition transitions_[NUM_TRANSITIONS] = {
        // Current State         Event                Next State              Action
        { HeaterState::OFF,      HeaterEvent::TURN_ON, HeaterState::HEATING,        [](HeaterStateTable& h){ h.turnHeaterOn(); } },
        { HeaterState::HEATING,  HeaterEvent::TURN_OFF, HeaterState::OFF,           [](HeaterStateTable& h){ h.turnHeaterOff(); } },
        { HeaterState::HEATING,  HeaterEvent::TEMP_OK, HeaterState::TARGET_REACHED, [](HeaterStateTable& h){ h.turnHeaterOff(); } },
        { HeaterState::TARGET_REACHED, HeaterEvent::TURN_OFF, HeaterState::OFF,     nullptr },
        { HeaterState::TARGET_REACHED, HeaterEvent::TEMP_LOW, HeaterState::HEATING, [](HeaterStateTable& h){ h.turnHeaterOn(); } },
    };

    HeaterState state_;
    bool heaterOn_;
};

// ============================================================================
// APPROACH 3: State Pattern (GoF / OO approach)
// ============================================================================

// Forward declaration
class HeaterContext;

/**
 * @brief Abstract state interface
 */
class IHeaterState {
public:
    virtual ~IHeaterState() = default;
    virtual void onEnter(HeaterContext& context) = 0;
    virtual void onExit(HeaterContext& context) = 0;
    virtual void handleTurnOn(HeaterContext& context) = 0;
    virtual void handleTurnOff(HeaterContext& context) = 0;
    virtual void handleTempLow(HeaterContext& context) = 0;
    virtual void handleTempOk(HeaterContext& context) = 0;
    virtual HeaterState getStateId() const = 0;
};

/**
 * @brief Context that holds current state and delegates events
 */
class HeaterContext {
public:
    HeaterContext();

    void handleEvent(HeaterEvent event);
    void transitionTo(IHeaterState* newState);

    HeaterState getState() const;
    bool isHeaterOn() const { return heaterOn_; }

    // Hardware actions (called by states)
    void activateHeater() { heaterOn_ = true; }
    void deactivateHeater() { heaterOn_ = false; }

private:
    IHeaterState* currentState_;
    bool heaterOn_;
};

/**
 * @brief OFF state
 */
class OffState : public IHeaterState {
public:
    static OffState& getInstance() {
        static OffState instance;
        return instance;
    }

    void onEnter(HeaterContext& /*context*/) override {}
    void onExit(HeaterContext& /*context*/) override {}

    void handleTurnOn(HeaterContext& context) override;
    void handleTurnOff(HeaterContext& /*context*/) override {}  // Ignore
    void handleTempLow(HeaterContext& /*context*/) override {}  // Ignore
    void handleTempOk(HeaterContext& /*context*/) override {}   // Ignore

    HeaterState getStateId() const override { return HeaterState::OFF; }

private:
    OffState() = default;
};

/**
 * @brief HEATING state
 */
class HeatingState : public IHeaterState {
public:
    static HeatingState& getInstance() {
        static HeatingState instance;
        return instance;
    }

    void onEnter(HeaterContext& context) override {
        context.activateHeater();
    }

    void onExit(HeaterContext& context) override {
        context.deactivateHeater();
    }

    void handleTurnOn(HeaterContext& /*context*/) override {}   // Already on
    void handleTurnOff(HeaterContext& context) override;
    void handleTempLow(HeaterContext& /*context*/) override {}  // Keep heating
    void handleTempOk(HeaterContext& context) override;

    HeaterState getStateId() const override { return HeaterState::HEATING; }

private:
    HeatingState() = default;
};

/**
 * @brief TARGET_REACHED state
 */
class TargetReachedState : public IHeaterState {
public:
    static TargetReachedState& getInstance() {
        static TargetReachedState instance;
        return instance;
    }

    void onEnter(HeaterContext& /*context*/) override {}
    void onExit(HeaterContext& /*context*/) override {}

    void handleTurnOn(HeaterContext& /*context*/) override {}   // Ignore
    void handleTurnOff(HeaterContext& context) override;
    void handleTempLow(HeaterContext& context) override;
    void handleTempOk(HeaterContext& /*context*/) override {}   // Already at target

    HeaterState getStateId() const override { return HeaterState::TARGET_REACHED; }

private:
    TargetReachedState() = default;
};

// ============================================================================
// State Pattern Implementation
// ============================================================================

inline HeaterContext::HeaterContext()
    : currentState_(&OffState::getInstance()), heaterOn_(false) {}

inline void HeaterContext::handleEvent(HeaterEvent event) {
    switch (event) {
        case HeaterEvent::TURN_ON:  currentState_->handleTurnOn(*this); break;
        case HeaterEvent::TURN_OFF: currentState_->handleTurnOff(*this); break;
        case HeaterEvent::TEMP_LOW: currentState_->handleTempLow(*this); break;
        case HeaterEvent::TEMP_OK:  currentState_->handleTempOk(*this); break;
    }
}

inline void HeaterContext::transitionTo(IHeaterState* newState) {
    currentState_->onExit(*this);
    currentState_ = newState;
    currentState_->onEnter(*this);
}

inline HeaterState HeaterContext::getState() const {
    return currentState_->getStateId();
}

// State transitions
inline void OffState::handleTurnOn(HeaterContext& context) {
    context.transitionTo(&HeatingState::getInstance());
}

inline void HeatingState::handleTurnOff(HeaterContext& context) {
    context.transitionTo(&OffState::getInstance());
}

inline void HeatingState::handleTempOk(HeaterContext& context) {
    context.transitionTo(&TargetReachedState::getInstance());
}

inline void TargetReachedState::handleTurnOff(HeaterContext& context) {
    context.transitionTo(&OffState::getInstance());
}

inline void TargetReachedState::handleTempLow(HeaterContext& context) {
    context.transitionTo(&HeatingState::getInstance());
}

}  // namespace state_pattern

#endif  // STATE_PATTERN_HPP
