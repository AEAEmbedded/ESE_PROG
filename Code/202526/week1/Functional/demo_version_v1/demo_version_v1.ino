/*
 * Arduino Stepper Motor Control - Structured Programming with Deceleration
 * Uses Arduino libraries with structured function design
 * Compatible with 8-bit 16MHz Arduino boards
 * 
 * v1.1 - Added proper deceleration for direction changes
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
const int MICROSTEPS = 8;  // Changed from 16 to 8
const long STEPS_PER_REV = (long)FULL_STEPS_PER_REV * MICROSTEPS;  // Now 1600 steps/rev

// --- Speed control ---
const float TARGET_RPM = 300.0;  // Increased from 120 to 300 RPM
const float ACCEL_RPM_S = 600.0;  // Increased acceleration

// Timing calculations
const float TARGET_SPS = (TARGET_RPM / 60.0f) * (float)STEPS_PER_REV;
const unsigned long TARGET_STEP_INTERVAL = 1000000UL / (unsigned long)TARGET_SPS; // microseconds
const unsigned long ACCEL_STEP_SIZE = 25; // Reduced from 50 - smoother acceleration at higher speeds
const uint8_t DECEL_MULTIPLIER = 5;

// --- NEW: Deceleration parameters ---
const long DECEL_DISTANCE = 400;  // Increased from 200 - need more distance at higher speeds
const unsigned long MIN_STEP_INTERVAL = TARGET_STEP_INTERVAL * 15; // Reduced multiplier for faster direction changes

// --- Motion state variables ---
volatile long currentPosition = 0;
volatile long targetPosition = STEPS_PER_REV;
volatile unsigned long currentStepInterval = TARGET_STEP_INTERVAL * 10; // start slow

// --- NEW: Motion state tracking ---
typedef enum {
  ACCELERATING,
  CONSTANT_SPEED,
  DECELERATING,
  DIRECTION_CHANGE
} MotionState;

volatile MotionState motionState = ACCELERATING;

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

// NEW: Motion state management
void updateMotionState(void);

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
void decelerateMotor(void); // NEW

// Timing utilities
unsigned long getCurrentTime(void);
boolean hasTimeElapsed(unsigned long currentTime, unsigned long previousTime, unsigned long interval);

// Utility functions
void printMotionStatus(void);
void debugPosition(void);

void setup() {
  Serial.begin(115200);
  performSystemSetup();
  Serial.println("Structured Arduino stepper control with deceleration started");
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
  motionState = ACCELERATING;
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
      updateMotionState();  // NEW: Update motion state before step
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
  
  // Only change direction if we're moving slowly enough
  if (currentStepInterval >= MIN_STEP_INTERVAL) {
    reverseDirection();
    motionState = DIRECTION_CHANGE;
    updateDirection();
    
    // Add a small pause to ensure complete stop
    delay(5); // Reduced from 10ms
    
    // Reset to acceleration phase for new direction
    motionState = ACCELERATING;
    currentStepInterval = MIN_STEP_INTERVAL; // Start from slow speed
    Serial.println("Direction changed - starting acceleration");
  }
}

// === NEW: MOTION STATE MANAGEMENT ===

void updateMotionState(void) {
  long stepsToTarget = abs(targetPosition - currentPosition);
  
  switch(motionState) {
    case ACCELERATING:
      if (currentStepInterval <= TARGET_STEP_INTERVAL) {
        motionState = CONSTANT_SPEED;
        Serial.println("Entering constant speed");
      } else if (stepsToTarget <= DECEL_DISTANCE) {
        motionState = DECELERATING;
        Serial.println("Starting deceleration");
      }
      break;
      
    case CONSTANT_SPEED:
      if (stepsToTarget <= DECEL_DISTANCE) {
        motionState = DECELERATING;
        Serial.println("Starting deceleration");
      }
      break;
      
    case DECELERATING:
      // Continue decelerating until very slow
      break;
      
    case DIRECTION_CHANGE:
      // Will be reset by handleTargetReached
      break;
  }
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
  delayMicroseconds(3); // Reduced from 5 - still above TB6600 minimum of 2.5μs
}

void createLowPulse(void) {
  digitalWrite(STEP_PIN, LOW);
  delayMicroseconds(3); // Reduced from 5 - less overhead at high speeds
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
  delayMicroseconds(50);       // Reduced from 100μs - still safe but less overhead at high speeds
}

// === SPEED/ACCELERATION CONTROL ===

void handleAcceleration(void) {
  switch(motionState) {
    case ACCELERATING:
      if (canAccelerate()) {
        accelerateMotor();
      }
      break;
      
    case CONSTANT_SPEED:
      // Maintain current speed
      break;
      
    case DECELERATING:
      decelerateMotor();
      break;
      
    case DIRECTION_CHANGE:
      // Very slow speed for direction change
      currentStepInterval = MIN_STEP_INTERVAL;
      break;
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

// NEW: Deceleration function
void decelerateMotor(void) {
  currentStepInterval += ACCEL_STEP_SIZE;  // Increase interval = slower speed
  if (currentStepInterval > MIN_STEP_INTERVAL) {
    currentStepInterval = MIN_STEP_INTERVAL;
  }
}

void resetSpeedForDirectionChange(void) {
  currentStepInterval = MIN_STEP_INTERVAL;  // Start very slow
  motionState = ACCELERATING;
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
  Serial.print(currentStepInterval);
  Serial.print(" State: ");
  Serial.println(motionState);
}

void debugPosition(void) {
  Serial.print("Direction changed. Position: ");
  Serial.print(currentPosition);
  Serial.print(" New target: ");
  Serial.print(targetPosition);
  Serial.print(" Speed interval: ");
  Serial.println(currentStepInterval);
}