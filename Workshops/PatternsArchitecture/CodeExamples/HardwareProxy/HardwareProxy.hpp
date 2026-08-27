#ifndef HARDWARE_PROXY_HPP
#define HARDWARE_PROXY_HPP

#include <cstdint>

/**
 * =============================================================================
 * HARDWARE PROXY PATTERN (also known as Hardware Abstraction Layer - HAL)
 * =============================================================================
 *
 * Problem:
 *   Code that directly accesses hardware registers cannot be tested off-target.
 *   Example: GPIO_PORTA->ODR |= (1 << 5);  // Can't run this on your laptop!
 *
 * Solution:
 *   Create an abstract interface for hardware access. Provide two implementations:
 *   1. Real implementation (talks to actual hardware)
 *   2. Mock/Fake implementation (for testing)
 *
 * Clean Architecture (Ch.29):
 *   "The hardware is a detail. A clean embedded architecture's software is
 *    testable OFF the target hardware."
 *
 * Douglass (Ch.3):
 *   "The Hardware Proxy pattern encapsulates the interface to hardware devices
 *    behind a software interface."
 *
 * =============================================================================
 */

namespace hardware_proxy {

// ============================================================================
// Abstract Interface (the "proxy")
// ============================================================================

/**
 * @brief Abstract LED interface - hardware is hidden behind this
 */
class ILed {
public:
    virtual ~ILed() = default;
    virtual void turnOn() = 0;
    virtual void turnOff() = 0;
    virtual void toggle() = 0;
    virtual bool isOn() const = 0;
};

/**
 * @brief Abstract GPIO interface for digital I/O
 */
class IGpio {
public:
    virtual ~IGpio() = default;
    virtual void setHigh() = 0;
    virtual void setLow() = 0;
    virtual bool read() const = 0;
};

// ============================================================================
// Real Implementation (for target hardware)
// ============================================================================

#ifdef STM32  // Only compile on real hardware

/**
 * @brief Real LED implementation for STM32
 *
 * This talks to actual hardware registers.
 * It will NOT compile on your development PC!
 */
class Stm32Led : public ILed {
public:
    Stm32Led(GPIO_TypeDef* port, uint16_t pin)
        : port_(port), pin_(pin), state_(false) {}

    void turnOn() override {
        HAL_GPIO_WritePin(port_, pin_, GPIO_PIN_SET);
        state_ = true;
    }

    void turnOff() override {
        HAL_GPIO_WritePin(port_, pin_, GPIO_PIN_RESET);
        state_ = false;
    }

    void toggle() override {
        HAL_GPIO_TogglePin(port_, pin_);
        state_ = !state_;
    }

    bool isOn() const override { return state_; }

private:
    GPIO_TypeDef* port_;
    uint16_t pin_;
    bool state_;
};

#endif  // STM32

// ============================================================================
// Mock Implementation (for unit testing)
// ============================================================================

/**
 * @brief Mock LED for unit testing
 *
 * This runs on your development PC without any hardware.
 * You can verify the LED was turned on/off in your tests.
 */
class MockLed : public ILed {
public:
    MockLed() : state_(false), toggleCount_(0) {}

    void turnOn() override { state_ = true; }
    void turnOff() override { state_ = false; }
    void toggle() override {
        state_ = !state_;
        toggleCount_++;
    }
    bool isOn() const override { return state_; }

    // Test helpers
    int getToggleCount() const { return toggleCount_; }
    void reset() { state_ = false; toggleCount_ = 0; }

private:
    bool state_;
    int toggleCount_;
};

// ============================================================================
// Application Code (uses the interface, not the implementation)
// ============================================================================

/**
 * @brief Heartbeat indicator - blinks an LED to show system is alive
 *
 * This class has NO knowledge of hardware. It only knows about ILed.
 * Therefore, it can be tested without any hardware!
 */
class HeartbeatIndicator {
public:
    explicit HeartbeatIndicator(ILed& led) : led_(led), beatCount_(0) {}

    void beat() {
        led_.toggle();
        beatCount_++;
    }

    int getBeatCount() const { return beatCount_; }

private:
    ILed& led_;
    int beatCount_;
};

/**
 * @brief Error indicator with different blink patterns
 */
class ErrorIndicator {
public:
    explicit ErrorIndicator(ILed& led) : led_(led) {}

    void showError() {
        // Quick double-blink for error
        led_.turnOn();
        // delay would go here
        led_.turnOff();
        // delay
        led_.turnOn();
        // delay
        led_.turnOff();
    }

    void showOk() {
        led_.turnOff();
    }

private:
    ILed& led_;
};

}  // namespace hardware_proxy

#endif  // HARDWARE_PROXY_HPP
