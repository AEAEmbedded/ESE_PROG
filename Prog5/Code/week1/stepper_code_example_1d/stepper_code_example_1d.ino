/*
 * Arduino Stepper Motor Control - Structured Programming
 * Uses Arduino libraries with structured function design
 * Compatible with 8-bit 16MHz Arduino boards
 * 
 * v1.0
 * Aug 2025
 * Embedded Programming (Prog 5/6)
 * johan.korten@han.nl
 * MIC2 Style
 */

// --- Pin definitions ---
const uint8_t STEP_PIN = 7;  // TB6600 PUL
const uint8_t DIR_PIN = 6;   // TB6600 DIR  
const uint8_t EN_PIN = 5;    // TB6600 ENA

// --- Motor/driver parameters ---
const int FULL_STEPS_PER_REV = 200;
const int MICROSTEPS = 16;
const long STEPS_PER_REV = (long)FULL_STEPS_PER_REV * MICROSTEPS;

// --- Speed control ---
const float TARGET_RPM = 120.0;
const float ACCEL_RPM_S = 300.0;

// Timing calculations
const float TARGET_SPS = (TARGET_RPM / 60.0f) * (float)STEPS_PER_REV;
const unsigned long TARGET_STEP_INTERVAL = 1000000UL / (unsigned long)TARGET_SPS; // microseconds
const unsigned long ACCEL_STEP_SIZE = 50; // microseconds
const uint8_t DECEL_MULTIPLIER = 5;

// --- Motion state variables ---
volatile long currentPosition = 0;
volatile long targetPosition = STEPS_PER_REV;
volatile unsigned long currentStepInterval = TARGET_STEP_INTERVAL * 10; // start slow

// --- Function prototypes ---
// System setup
void performSystemSetup(void);
void initializeHardware(void);
void initializePins(void);
void enableDriverOutput(void);
void initializeMotionParameters(void);

// Main motion control
void executeMotionControl(void);
boolean isTimeForNextStep(unsigned long currentTime, unsigned long *lastStepTime);
void executeStepSequence(unsigned long currentTime, unsigned long *lastStepTime);
void handleTargetReached(void);

// Position management
void updatePosition(void);
boolean hasReachedTarget(void);
void reverseDirection(void);
long calculateNewTarget(void);
boolean isMovingForward(void);

// Step generation
void makeStep(void);
void generateStepPulse(void);
void createHighPulse(void);
void createLowPulse(void);

// Direction control
void updateDirection(void);
void setDirectionForward(void);
void setDirectionReverse(void);
void waitForDirectionSetup(void);

// Speed/acceleration control
void handleAcceleration(void);
void accelerateMotor(void);
boolean canAccelerate(void);
void resetSpeedForDirectionChange(void);

// Timing utilities
unsigned long getCurrentTime(void);
boolean hasTimeElapsed(unsigned long currentTime, unsigned long previousTime, unsigned long interval);

// Utility functions
void printMotionStatus(void);
void debugPosition(void);

void setup() {
  Serial.begin(115200);
  performSystemSetup();
  Serial.println("Structured Arduino stepper control started");
}

void loop() {
  executeMotionControl();
}

// === SYSTEM SETUP FUNCTIONS ===

void performSystemSetup(void) {
  initializeHardware();
  initializeMotionParameters();
}

void initializeHardware(void) {
  initializePins();
  enableDriverOutput();
}

void initializePins(void) {
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(EN_PIN, OUTPUT);
  
  digitalWrite(STEP_PIN, LOW);  // STEP low
  digitalWrite(DIR_PIN, LOW);   // DIR low (forward)
}

void enableDriverOutput(void) {
  digitalWrite(EN_PIN, HIGH);   // EN high (enable driver)
}

void initializeMotionParameters(void) {
  currentPosition = 0;
  targetPosition = STEPS_PER_REV;
  currentStepInterval = TARGET_STEP_INTERVAL * 10;
  updateDirection();
}

// === MAIN MOTION CONTROL ===

void executeMotionControl(void) {
  static unsigned long lastStepTime = 0;
  unsigned long currentTime = getCurrentTime();
  
  if (isTimeForNextStep(currentTime, &lastStepTime)) {
    if (hasReachedTarget()) {
      handleTargetReached();
    } else {
      executeStepSequence(currentTime, &lastStepTime);
    }
  }
}

boolean isTimeForNextStep(unsigned long currentTime, unsigned long *lastStepTime) {
  return hasTimeElapsed(currentTime, *lastStepTime, currentStepInterval);
}

void executeStepSequence(unsigned long currentTime, unsigned long *lastStepTime) {
  makeStep();
  *lastStepTime = currentTime;
  updatePosition();
  handleAcceleration();
}

void handleTargetReached(void) {
  debugPosition();
  reverseDirection();
  resetSpeedForDirectionChange();
  updateDirection();
}

// === POSITION MANAGEMENT ===

void updatePosition(void) {
  if (isMovingForward()) {
    currentPosition++;
  } else {
    currentPosition--;
  }
}

boolean hasReachedTarget(void) {
  return (currentPosition == targetPosition);
}

void reverseDirection(void) {
  targetPosition = calculateNewTarget();
}

long calculateNewTarget(void) {
  return -currentPosition; // ping-pong motion
}

boolean isMovingForward(void) {
  return (targetPosition > currentPosition);
}

// === STEP GENERATION ===

void makeStep(void) {
  generateStepPulse();
}

void generateStepPulse(void) {
  createHighPulse();
  createLowPulse();
}

void createHighPulse(void) {
  digitalWrite(STEP_PIN, HIGH);
  delayMicroseconds(5); // Minimum pulse width for TB6600
}

void createLowPulse(void) {
  digitalWrite(STEP_PIN, LOW);
  delayMicroseconds(5); // Minimum low time
}

// === DIRECTION CONTROL ===

void updateDirection(void) {
  if (isMovingForward()) {
    setDirectionForward();
  } else {
    setDirectionReverse();
  }
  waitForDirectionSetup();
}

void setDirectionForward(void) {
  digitalWrite(DIR_PIN, LOW);  // DIR low = forward
}

void setDirectionReverse(void) {
  digitalWrite(DIR_PIN, HIGH); // DIR high = reverse
}

void waitForDirectionSetup(void) {
  delayMicroseconds(20);       // Direction setup time for TB6600
}

// === SPEED/ACCELERATION CONTROL ===

void handleAcceleration(void) {
  if (canAccelerate()) {
    accelerateMotor();
  }
}

boolean canAccelerate(void) {
  return (currentStepInterval > TARGET_STEP_INTERVAL);
}

void accelerateMotor(void) {
  currentStepInterval -= ACCEL_STEP_SIZE;
  if (currentStepInterval < TARGET_STEP_INTERVAL) {
    currentStepInterval = TARGET_STEP_INTERVAL;
  }
}

void resetSpeedForDirectionChange(void) {
  currentStepInterval = TARGET_STEP_INTERVAL * DECEL_MULTIPLIER;
}

// === TIMING UTILITIES ===

unsigned long getCurrentTime(void) {
  return micros();
}

boolean hasTimeElapsed(unsigned long currentTime, unsigned long previousTime, unsigned long interval) {
  return (currentTime - previousTime) >= interval;
}

// === UTILITY FUNCTIONS ===

void printMotionStatus(void) {
  Serial.print("Position: ");
  Serial.print(currentPosition);
  Serial.print(" Target: ");
  Serial.print(targetPosition);
  Serial.print(" Interval: ");
  Serial.println(currentStepInterval);
}

void debugPosition(void) {
  Serial.print("Direction changed. Position: ");
  Serial.print(currentPosition);
  Serial.print(" New target: ");
  Serial.println(targetPosition);
}
