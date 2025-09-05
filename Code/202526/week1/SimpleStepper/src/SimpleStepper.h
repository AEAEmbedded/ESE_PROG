/*
  SimpleStepper.h - Clean library for controlling stepper motors with TB6600 drivers
  Created for Embedded Programming Course, HAN University
  Version 2.0.0
  Aug 2025
  
  Note: Configured for active-low control (PUL-, DIR-, ENA-)
*/

#ifndef SimpleStepper_h
#define SimpleStepper_h

#include "Arduino.h"

// Proper enum for direction (type safety)
enum class Direction : uint8_t {
  CLOCKWISE = 0,
  COUNTER_CLOCKWISE = 1
};

// Enum for motor state
enum class MotorState : uint8_t {
  DISABLED = 0,
  ENABLED = 1
};

// Enum for signal logic type
enum class SignalLogic : uint8_t {
  ACTIVE_HIGH = 0,  // Traditional: HIGH = active (PUL+, DIR+, ENA+)
  ACTIVE_LOW = 1    // Inverted: LOW = active (PUL-, DIR-, ENA-)
};

// Configuration structure (SRP: separate configuration from control)
struct MotorConfig {
  uint16_t stepsPerRevolution = 200;
  uint8_t microsteps = 16;
  uint16_t rpm = 60;
  SignalLogic signalLogic = SignalLogic::ACTIVE_LOW;  // Default to active-low
};

class SimpleStepper {
  public:
    // Constructor
    SimpleStepper(uint8_t stepPin, uint8_t dirPin, uint8_t enablePin);
    
    // Initialization (single method with optional config)
    void begin(const MotorConfig& config = MotorConfig());
    
    // Motor state control (SRP: state management)
    void setState(MotorState state);
    MotorState getState() const;
    
    // Direction control
    void setDirection(Direction dir);
    Direction getDirection() const;
    
    // Movement (KISS: simple, clear methods)
    void step();
    void move(uint32_t steps);
    void rotate(float revolutions);
    
    // Speed configuration
    void setRPM(uint16_t rpm);
    uint16_t getRPM() const;
    
  private:
    // Pin assignments (const after initialization)
    const uint8_t _stepPin;
    const uint8_t _dirPin;
    const uint8_t _enablePin;
    
    // Current state
    Direction _direction;
    MotorState _state;
    
    // Configuration
    MotorConfig _config;
    uint32_t _stepDelayMicros;
    
    // Private methods (SRP: each method has one job)
    void updateStepDelay();
    void pulseStep();
    void delayMicros(uint32_t micros);
    void setPinStates();
    uint32_t calculateTotalSteps(float revolutions) const;
    
    // Logic level helpers (DRY: centralize logic inversion)
    inline uint8_t activeLevel() const { 
      return (_config.signalLogic == SignalLogic::ACTIVE_LOW) ? LOW : HIGH; 
    }
    inline uint8_t inactiveLevel() const { 
      return (_config.signalLogic == SignalLogic::ACTIVE_LOW) ? HIGH : LOW; 
    }
};

// Convenience aliases for backward compatibility
using CW = Direction;
using CCW = Direction;
constexpr Direction CLOCKWISE = Direction::CLOCKWISE;
constexpr Direction COUNTER_CLOCKWISE = Direction::COUNTER_CLOCKWISE;

#endif