#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <cstdint>
#include <array>

/**
 * =============================================================================
 * OBSERVER PATTERN FOR EMBEDDED SYSTEMS
 * =============================================================================
 *
 * Problem:
 *   - Polling wastes CPU cycles checking for changes
 *   - Tight coupling: module A directly calls module B when something happens
 *   - ISR (Interrupt Service Routine) needs to notify application code
 *
 * Solution:
 *   Observers register interest in a subject. When the subject changes,
 *   it notifies all observers. Decouples the "what happened" from
 *   "who needs to know".
 *
 * Embedded considerations:
 *   - No dynamic memory allocation (fixed-size observer list)
 *   - ISR-safe notification (keep it short!)
 *   - Consider deferred processing (set flag in ISR, process in main loop)
 *
 * Douglass (Ch.3): Observer Pattern for hardware events
 *
 * =============================================================================
 */

namespace observer {

// ============================================================================
// Observer Interface
// ============================================================================

/**
 * @brief Interface for button press observers
 */
class IButtonObserver {
public:
    virtual ~IButtonObserver() = default;
    virtual void onButtonPressed(uint8_t buttonId) = 0;
    virtual void onButtonReleased(uint8_t buttonId) = 0;
};

/**
 * @brief Interface for temperature observers
 */
class ITemperatureObserver {
public:
    virtual ~ITemperatureObserver() = default;
    virtual void onTemperatureChanged(float celsius) = 0;
    virtual void onOvertemperature(float celsius) = 0;
};

// ============================================================================
// Subject (Observable)
// ============================================================================

/**
 * @brief Button subject - notifies observers of button events
 *
 * Note: Uses fixed-size array to avoid dynamic allocation.
 * This is typical for embedded systems.
 */
template<size_t MAX_OBSERVERS = 4>
class ButtonSubject {
public:
    ButtonSubject() : numObservers_(0) {}

    bool attach(IButtonObserver* observer) {
        if (numObservers_ >= MAX_OBSERVERS) {
            return false;  // No room
        }
        observers_[numObservers_++] = observer;
        return true;
    }

    bool detach(IButtonObserver* observer) {
        for (size_t i = 0; i < numObservers_; ++i) {
            if (observers_[i] == observer) {
                // Shift remaining observers
                for (size_t j = i; j < numObservers_ - 1; ++j) {
                    observers_[j] = observers_[j + 1];
                }
                numObservers_--;
                return true;
            }
        }
        return false;
    }

    // Called from ISR or polling loop when button state changes
    void notifyPressed(uint8_t buttonId) {
        for (size_t i = 0; i < numObservers_; ++i) {
            observers_[i]->onButtonPressed(buttonId);
        }
    }

    void notifyReleased(uint8_t buttonId) {
        for (size_t i = 0; i < numObservers_; ++i) {
            observers_[i]->onButtonReleased(buttonId);
        }
    }

    size_t getObserverCount() const { return numObservers_; }

private:
    std::array<IButtonObserver*, MAX_OBSERVERS> observers_;
    size_t numObservers_;
};

/**
 * @brief Temperature sensor subject
 */
template<size_t MAX_OBSERVERS = 4>
class TemperatureSubject {
public:
    TemperatureSubject(float threshold = 50.0f)
        : numObservers_(0), threshold_(threshold), lastTemp_(0.0f) {}

    bool attach(ITemperatureObserver* observer) {
        if (numObservers_ >= MAX_OBSERVERS) return false;
        observers_[numObservers_++] = observer;
        return true;
    }

    // Called when new temperature reading is available
    void updateTemperature(float celsius) {
        lastTemp_ = celsius;

        // Notify all observers of change
        for (size_t i = 0; i < numObservers_; ++i) {
            observers_[i]->onTemperatureChanged(celsius);
        }

        // Check for overtemperature
        if (celsius > threshold_) {
            for (size_t i = 0; i < numObservers_; ++i) {
                observers_[i]->onOvertemperature(celsius);
            }
        }
    }

    float getLastTemperature() const { return lastTemp_; }

private:
    std::array<ITemperatureObserver*, MAX_OBSERVERS> observers_;
    size_t numObservers_;
    float threshold_;
    float lastTemp_;
};

// ============================================================================
// Concrete Observers
// ============================================================================

/**
 * @brief LED controller that responds to button presses
 */
class LedController : public IButtonObserver {
public:
    LedController() : ledState_(false), pressCount_(0) {}

    void onButtonPressed(uint8_t /*buttonId*/) override {
        ledState_ = !ledState_;  // Toggle LED
        pressCount_++;
    }

    void onButtonReleased(uint8_t /*buttonId*/) override {
        // Could do something on release
    }

    bool isLedOn() const { return ledState_; }
    int getPressCount() const { return pressCount_; }

private:
    bool ledState_;
    int pressCount_;
};

/**
 * @brief Event logger that records button activity
 */
class EventLogger : public IButtonObserver {
public:
    static constexpr size_t LOG_SIZE = 16;

    EventLogger() : logIndex_(0) {}

    void onButtonPressed(uint8_t buttonId) override {
        if (logIndex_ < LOG_SIZE) {
            log_[logIndex_++] = {buttonId, true};
        }
    }

    void onButtonReleased(uint8_t buttonId) override {
        if (logIndex_ < LOG_SIZE) {
            log_[logIndex_++] = {buttonId, false};
        }
    }

    size_t getLogCount() const { return logIndex_; }

    struct LogEntry {
        uint8_t buttonId;
        bool pressed;
    };

    const LogEntry& getLogEntry(size_t index) const { return log_[index]; }

private:
    std::array<LogEntry, LOG_SIZE> log_;
    size_t logIndex_;
};

/**
 * @brief Display that shows temperature
 */
class TemperatureDisplay : public ITemperatureObserver {
public:
    TemperatureDisplay() : displayValue_(0.0f), alarmActive_(false) {}

    void onTemperatureChanged(float celsius) override {
        displayValue_ = celsius;
        // In real code: update LCD/7-segment here
    }

    void onOvertemperature(float /*celsius*/) override {
        alarmActive_ = true;
        // In real code: trigger alarm buzzer
    }

    float getDisplayValue() const { return displayValue_; }
    bool isAlarmActive() const { return alarmActive_; }
    void clearAlarm() { alarmActive_ = false; }

private:
    float displayValue_;
    bool alarmActive_;
};

/**
 * @brief Safety controller that responds to overtemperature
 */
class SafetyController : public ITemperatureObserver {
public:
    SafetyController() : shutdownTriggered_(false) {}

    void onTemperatureChanged(float /*celsius*/) override {
        // Normal operation, nothing to do
    }

    void onOvertemperature(float /*celsius*/) override {
        shutdownTriggered_ = true;
        // In real code: trigger emergency shutdown
    }

    bool isShutdownTriggered() const { return shutdownTriggered_; }
    void reset() { shutdownTriggered_ = false; }

private:
    bool shutdownTriggered_;
};

}  // namespace observer

#endif  // OBSERVER_HPP
