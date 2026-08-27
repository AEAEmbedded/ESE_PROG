/*
  SimpleStepper.cpp - Clean library for controlling stepper motors with TB6600 drivers
  Created for Embedded Programming Course, HAN University
  Version 2.0.0
  Aug 2025
  
  Note: Configured for active-low control (PUL-, DIR-, ENA-)
  Logic is inverted: LOW = active, HIGH = inactive
*/

#include "SimpleStepper.h"

// Constants (DRY principle - define once, use many)
namespace {
  constexpr uint8_t PULSE_WIDTH_MICROS = 5;
  constexpr uint8_t SETUP_TIME_MICROS = 20;
  constexpr uint16_t INIT_DELAY_MILLIS = 10;
  constexpr uint32_t MICROS_PER_MINUTE = 60000000UL;
  constexpr uint16_t MAX_DELAY_MICROS = 16383;
  constexpr uint16_t MILLIS_PER_SECOND = 1000;
}

// Constructor - initialization list (RAII principle)
SimpleStepper::SimpleStepper(uint8_t stepPin, uint8_t dirPin, uint8_t enablePin)
  : _stepPin(stepPin),
    _dirPin(dirPin),
    _enablePin(enablePin),
    _direction(Direction::CLOCKWISE),
    _state(MotorState::DISABLED),
    _config(),
    _stepDelayMicros(1000) {
}

// Initialize motor with configuration (SRP: only initialization)
void SimpleStepper::begin(const MotorConfig& config) {
  _config = config;
  
  // Setup pins (delegated to separate method - SRP)
  setPinStates();
  
  // Calculate timing
  updateStepDelay();
  
  // Enable motor by default
  setState(MotorState::ENABLED);
  
  // Allow driver initialization
  delay(INIT_DELAY_MILLIS);
}

// Set motor state (SRP: only state management)
void SimpleStepper::setState(MotorState state) {
  _state = state;
  // Use configured logic level (active-low or active-high)
  digitalWrite(_enablePin, state == MotorState::ENABLED ? activeLevel() : inactiveLevel());
  delayMicroseconds(SETUP_TIME_MICROS);
}

// Get current motor state
MotorState SimpleStepper::getState() const {
  return _state;
}

// Set direction (SRP: only direction management)
void SimpleStepper::setDirection(Direction dir) {
  _direction = dir;
  digitalWrite(_dirPin, static_cast<uint8_t>(dir));
  delayMicroseconds(SETUP_TIME_MICROS);
}

// Get current direction
Direction SimpleStepper::getDirection() const {
  return _direction;
}

// Generate single step (SRP: only step generation)
void SimpleStepper::step() {
  pulseStep();
  delayMicros(_stepDelayMicros);
}

// Move specified number of steps (uses DRY - calls step())
void SimpleStepper::move(uint32_t steps) {
  for (uint32_t i = 0; i < steps; i++) {
    step();
  }
}

// Rotate specified revolutions (SRP: only rotation logic)
void SimpleStepper::rotate(float revolutions) {
  uint32_t totalSteps = calculateTotalSteps(revolutions);
  move(totalSteps);
}

// Set speed in RPM (SRP: only speed management)
void SimpleStepper::setRPM(uint16_t rpm) {
  _config.rpm = rpm;
  updateStepDelay();
}

// Get current RPM
uint16_t SimpleStepper::getRPM() const {
  return _config.rpm;
}

// PRIVATE METHODS

// Initialize pin states (SRP: only pin initialization)
void SimpleStepper::setPinStates() {
  pinMode(_stepPin, OUTPUT);
  pinMode(_dirPin, OUTPUT);
  pinMode(_enablePin, OUTPUT);
  
  // Set initial states based on configured logic
  digitalWrite(_stepPin, inactiveLevel());  // Step inactive
  digitalWrite(_dirPin, static_cast<uint8_t>(_direction));
  digitalWrite(_enablePin, inactiveLevel());  // Start disabled
}

// Update step delay based on configuration (SRP: only delay calculation)
void SimpleStepper::updateStepDelay() {
  uint32_t stepsPerMinute = static_cast<uint32_t>(_config.rpm) * 
                            _config.stepsPerRevolution * 
                            _config.microsteps;
  
  if (stepsPerMinute > 0) {
    _stepDelayMicros = MICROS_PER_MINUTE / stepsPerMinute;
  }
}

// Generate step pulse (SRP: only pulse generation)
void SimpleStepper::pulseStep() {
  // Generate pulse based on configured logic
  digitalWrite(_stepPin, activeLevel());   // Active
  delayMicroseconds(PULSE_WIDTH_MICROS);
  digitalWrite(_stepPin, inactiveLevel());  // Inactive
}

// Handle delays > 16383 microseconds (SRP: only delay handling)
void SimpleStepper::delayMicros(uint32_t micros) {
  if (micros > MAX_DELAY_MICROS) {
    delay(micros / MILLIS_PER_SECOND);
    delayMicroseconds(micros % MILLIS_PER_SECOND);
  } else if (micros > 0) {
    delayMicroseconds(micros);
  }
}

// Calculate total steps for revolutions (SRP: only step calculation)
uint32_t SimpleStepper::calculateTotalSteps(float revolutions) const {
  return static_cast<uint32_t>(revolutions * 
                                _config.stepsPerRevolution * 
                                _config.microsteps);
}