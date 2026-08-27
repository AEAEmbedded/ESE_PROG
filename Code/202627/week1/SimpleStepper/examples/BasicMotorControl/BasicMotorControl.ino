/*
  BasicMotorControl.ino
  
  Clean example for SimpleStepper library v2.0
  Demonstrates stepper motor control using clean code principles
  
  Circuit (Active-Low Configuration):
  - Step Pin: D7 -> TB6600 PUL- (Active Low)
  - Direction Pin: D6 -> TB6600 DIR- (Active Low)
  - Enable Pin: D5 -> TB6600 ENA- (Active Low)
  - +5V -> TB6600 PUL+/DIR+/ENA+ (Common positive)
  - GND -> Arduino GND
  
  Note: Signals are active-low (LOW = active, HIGH = inactive)
  
  Created for Embedded Programming Course
  HAN University, Aug 2025
*/

#include <SimpleStepper.h>

// Pin configuration
constexpr uint8_t STEP_PIN = 7;
constexpr uint8_t DIR_PIN = 6;
constexpr uint8_t ENABLE_PIN = 5;

// Create stepper object
SimpleStepper stepper(STEP_PIN, DIR_PIN, ENABLE_PIN);

// Demo configuration
constexpr uint16_t DEMO_DELAY_MS = 500;
constexpr uint16_t LONG_DELAY_MS = 2000;

void setup() {
  Serial.begin(9600);
  Serial.println(F("SimpleStepper v2.0 - Clean Code Example"));
  
  // Configure motor (using struct for clarity)
  MotorConfig config;
  config.stepsPerRevolution = 200;
  config.microsteps = 16;
  config.rpm = 60;
  
  // Choose signal logic based on your wiring:
  // For PUL-, DIR-, ENA- connections (active-low):
  config.signalLogic = SignalLogic::ACTIVE_LOW;
  
  // For PUL+, DIR+, ENA+ connections (active-high):
  // config.signalLogic = SignalLogic::ACTIVE_HIGH;
  
  // Initialize with configuration
  stepper.begin(config);
  
  Serial.println(F("Motor initialized"));
  Serial.print(F("Signal Logic: "));
  Serial.println(config.signalLogic == SignalLogic::ACTIVE_LOW ? 
                 F("Active-Low (PUL-, DIR-, ENA-)") : 
                 F("Active-High (PUL+, DIR+, ENA+)"));
  Serial.print(F("Speed: ")); 
  Serial.print(stepper.getRPM());
  Serial.println(F(" RPM"));
  
  delay(1000);
}

void loop() {
  // Run demonstration sequence
  demonstrateBasicMovement();
  demonstrateSpeedControl();
  demonstratePowerManagement();
  demonstratePatternMovement();
  
  Serial.println(F("\n=== Demo Complete - Restarting in 5 seconds ===\n"));
  delay(5000);
}

// Demonstrate basic movement (SRP: one function, one purpose)
void demonstrateBasicMovement() {
  Serial.println(F("\n--- Basic Movement ---"));
  
  // Clockwise rotation
  Serial.println(F("Rotating 1 revolution clockwise"));
  stepper.setDirection(Direction::CLOCKWISE);
  stepper.rotate(1.0);
  delay(DEMO_DELAY_MS);
  
  // Counter-clockwise rotation
  Serial.println(F("Rotating 1 revolution counter-clockwise"));
  stepper.setDirection(Direction::COUNTER_CLOCKWISE);
  stepper.rotate(1.0);
  delay(DEMO_DELAY_MS);
  
  // Partial rotation
  Serial.println(F("Half revolution clockwise"));
  stepper.setDirection(Direction::CLOCKWISE);
  stepper.rotate(0.5);
  delay(DEMO_DELAY_MS);
}

// Demonstrate speed changes (SRP)
void demonstrateSpeedControl() {
  Serial.println(F("\n--- Speed Control ---"));
  
  // Store original speed
  uint16_t originalRPM = stepper.getRPM();
  
  // Test different speeds
  const uint16_t speeds[] = {30, 60, 120, 180};
  
  for (uint16_t speed : speeds) {
    Serial.print(F("Speed: "));
    Serial.print(speed);
    Serial.println(F(" RPM"));
    
    stepper.setRPM(speed);
    stepper.move(800);  // Quarter revolution at 16 microsteps
    delay(DEMO_DELAY_MS);
  }
  
  // Restore original speed
  stepper.setRPM(originalRPM);
}

// Demonstrate power management (SRP)
void demonstratePowerManagement() {
  Serial.println(F("\n--- Power Management ---"));
  
  // Normal operation
  Serial.println(F("Motor enabled - holding position"));
  stepper.setState(MotorState::ENABLED);
  delay(1000);
  
  // Power saving
  Serial.println(F("Motor disabled - power saving mode"));
  stepper.setState(MotorState::DISABLED);
  delay(LONG_DELAY_MS);
  
  // Re-enable
  Serial.println(F("Motor re-enabled"));
  stepper.setState(MotorState::ENABLED);
  delay(100);
}

// Demonstrate pattern movement (SRP)
void demonstratePatternMovement() {
  Serial.println(F("\n--- Pattern Movement ---"));
  
  // Square pattern (4 x 90 degree turns)
  Serial.println(F("Square pattern"));
  for (int i = 0; i < 4; i++) {
    stepper.rotate(0.25);  // 90 degrees
    delay(250);
  }
  
  // Back and forth pattern
  Serial.println(F("Oscillation pattern"));
  for (int i = 0; i < 3; i++) {
    stepper.setDirection(Direction::CLOCKWISE);
    stepper.move(400);
    stepper.setDirection(Direction::COUNTER_CLOCKWISE);
    stepper.move(400);
  }
  
  // Acceleration simulation (clean implementation)
  Serial.println(F("Acceleration pattern"));
  accelerateMotor(30, 150, 30);
  
  delay(DEMO_DELAY_MS);
}

// Helper function for acceleration (SRP: single responsibility)
void accelerateMotor(uint16_t startRPM, uint16_t endRPM, uint16_t stepRPM) {
  uint16_t currentRPM = stepper.getRPM();
  
  // Accelerate
  for (uint16_t rpm = startRPM; rpm <= endRPM; rpm += stepRPM) {
    stepper.setRPM(rpm);
    stepper.move(200);
  }
  
  // Decelerate
  for (uint16_t rpm = endRPM; rpm >= startRPM; rpm -= stepRPM) {
    stepper.setRPM(rpm);
    stepper.move(200);
  }
  
  // Restore original speed
  stepper.setRPM(currentRPM);
}