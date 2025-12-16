#ifndef DEBOUNCING_HPP
#define DEBOUNCING_HPP

#include <cstdint>

/**
 * =============================================================================
 * DEBOUNCING PATTERN FOR EMBEDDED SYSTEMS
 * =============================================================================
 *
 * Problem:
 *   Mechanical switches/buttons "bounce" when pressed - they rapidly
 *   open and close for several milliseconds before settling. This causes
 *   multiple false triggers if not handled properly.
 *
 *   Typical bounce duration: 5-50ms depending on switch quality
 *
 * Solutions shown here:
 *   1. DELAY-BASED: Wait for signal to stabilize
 *   2. COUNTER-BASED: Require N consecutive same readings
 *   3. INTEGRATOR: Accumulate evidence for state change
 *
 * Douglass (Ch.3): Debouncing Pattern
 *
 * =============================================================================
 */

namespace debouncing {

// ============================================================================
// Raw button interface (for testing)
// ============================================================================

class IRawButton {
public:
    virtual ~IRawButton() = default;
    virtual bool readRaw() const = 0;  // true = pressed
};

// ============================================================================
// APPROACH 1: Delay-Based Debouncing
// ============================================================================

/**
 * @brief Simple delay-based debouncer
 *
 * When a change is detected, wait for debounce period before reporting.
 * Call update() periodically (e.g., every 1ms from timer ISR).
 *
 * Pros: Simple to understand
 * Cons: Adds latency to button response
 */
class DelayDebouncer {
public:
    DelayDebouncer(IRawButton& button, uint16_t debounceMs = 20)
        : button_(button)
        , debounceMs_(debounceMs)
        , counter_(0)
        , lastRaw_(false)
        , debouncedState_(false)
        , stateChanged_(false)
    {}

    /**
     * @brief Call this every 1ms (e.g., from SysTick handler)
     */
    void update() {
        bool currentRaw = button_.readRaw();
        stateChanged_ = false;

        if (currentRaw != lastRaw_) {
            // State changed, start debounce timer
            counter_ = 0;
            lastRaw_ = currentRaw;
        } else {
            // State stable, increment counter
            if (counter_ < debounceMs_) {
                counter_++;
                if (counter_ >= debounceMs_) {
                    // Debounce period elapsed, accept new state
                    if (currentRaw != debouncedState_) {
                        debouncedState_ = currentRaw;
                        stateChanged_ = true;
                    }
                }
            }
        }
    }

    bool isPressed() const { return debouncedState_; }
    bool stateChanged() const { return stateChanged_; }

    // For testing
    uint16_t getCounter() const { return counter_; }

private:
    IRawButton& button_;
    uint16_t debounceMs_;
    uint16_t counter_;
    bool lastRaw_;
    bool debouncedState_;
    bool stateChanged_;
};

// ============================================================================
// APPROACH 2: Counter-Based Debouncing (Vertical Counter / Shift Register)
// ============================================================================

/**
 * @brief Counter-based debouncer using bit history
 *
 * Keeps a history of the last N readings. Only changes state when
 * all N readings are the same.
 *
 * Uses 8-bit shift register = requires 8 consecutive same readings
 *
 * Pros: Very efficient (bitwise ops), configurable history depth
 * Cons: Requires consistent polling rate
 */
class ShiftRegisterDebouncer {
public:
    ShiftRegisterDebouncer(IRawButton& button)
        : button_(button)
        , history_(0x00)  // Start assuming released
        , debouncedState_(false)
        , stateChanged_(false)
    {}

    /**
     * @brief Call this every 1-5ms
     */
    void update() {
        stateChanged_ = false;

        // Shift history left, add new reading at bit 0
        history_ = (history_ << 1) | (button_.readRaw() ? 0x01 : 0x00);

        // Check for all 1s (pressed) or all 0s (released)
        if (history_ == 0xFF) {
            if (!debouncedState_) {
                debouncedState_ = true;
                stateChanged_ = true;
            }
        } else if (history_ == 0x00) {
            if (debouncedState_) {
                debouncedState_ = false;
                stateChanged_ = true;
            }
        }
        // Otherwise: mixed readings, state unchanged
    }

    bool isPressed() const { return debouncedState_; }
    bool stateChanged() const { return stateChanged_; }

    // For testing/debugging
    uint8_t getHistory() const { return history_; }

private:
    IRawButton& button_;
    uint8_t history_;
    bool debouncedState_;
    bool stateChanged_;
};

// ============================================================================
// APPROACH 3: Integrator Debouncing
// ============================================================================

/**
 * @brief Integrator-based debouncer
 *
 * Maintains a counter that increments when pressed, decrements when released.
 * State changes only when counter reaches limits.
 *
 * Pros: Smooth handling of noise, adjustable sensitivity
 * Cons: Slightly more complex, more memory
 */
class IntegratorDebouncer {
public:
    IntegratorDebouncer(IRawButton& button, uint8_t maxCount = 10)
        : button_(button)
        , maxCount_(maxCount)
        , counter_(0)
        , debouncedState_(false)
        , stateChanged_(false)
    {}

    /**
     * @brief Call this every 1-5ms
     */
    void update() {
        stateChanged_ = false;

        if (button_.readRaw()) {
            // Button pressed - increment counter
            if (counter_ < maxCount_) {
                counter_++;
                if (counter_ >= maxCount_) {
                    if (!debouncedState_) {
                        debouncedState_ = true;
                        stateChanged_ = true;
                    }
                }
            }
        } else {
            // Button released - decrement counter
            if (counter_ > 0) {
                counter_--;
                if (counter_ == 0) {
                    if (debouncedState_) {
                        debouncedState_ = false;
                        stateChanged_ = true;
                    }
                }
            }
        }
    }

    bool isPressed() const { return debouncedState_; }
    bool stateChanged() const { return stateChanged_; }

    // For testing
    uint8_t getCounter() const { return counter_; }

private:
    IRawButton& button_;
    uint8_t maxCount_;
    uint8_t counter_;
    bool debouncedState_;
    bool stateChanged_;
};

// ============================================================================
// Mock button for testing
// ============================================================================

class MockButton : public IRawButton {
public:
    MockButton() : pressed_(false) {}

    bool readRaw() const override { return pressed_; }

    // Test control
    void press() { pressed_ = true; }
    void release() { pressed_ = false; }
    void setState(bool pressed) { pressed_ = pressed; }

private:
    bool pressed_;
};

}  // namespace debouncing

#endif  // DEBOUNCING_HPP
