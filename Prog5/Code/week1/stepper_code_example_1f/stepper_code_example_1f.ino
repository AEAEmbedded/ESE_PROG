/*
 * Arduino Stepper Motor Control - Memory Optimized
 * Balanced SOLID principles with memory efficiency
 * Compatible with 8-bit 16MHz Arduino boards
 * 
 * v1.0f - Memory-optimized version with state machine
 * - Reduced virtual function overhead
 * - PROGMEM for string constants
 * - Simplified class hierarchy
 * - Maintains core functionality
 * 
 * Sep 2025
 * Embedded Programming (Prog 5/6)
 * SOLID Principles balanced with performance
 */

#include <avr/pgmspace.h>

// === PIN CONFIGURATION ===
#define STEP_PIN 7
#define DIR_PIN 6
#define ENABLE_PIN 5
#define SENSOR_PIN 12

// === MOTOR CONFIGURATION ===
#define FULL_STEPS_PER_REV 200
#define MICROSTEPS 8
#define TARGET_RPM 180.0f  // Moderate speed increase

// === MOTION PARAMETERS ===
#define HOMING_SPEED_DIVISOR 2  // Slower but not too slow for homing
#define ACCEL_SPEED_DIVISOR 5
#define ACCEL_STEP_SIZE 50
#define SENSOR_BACKOFF_STEPS 200
#define DEFAULT_TARGET_POSITION 3200

// === TIMING CONSTANTS ===
#define PULSE_WIDTH_US 10  // Standard pulse width for TB6600
#define DIRECTION_SETUP_TIME_US 20

#define MAX_POSITION 22000
#define MIN_POSITION 0

// M2M communication messages - structured format
const char MSG_INIT[] PROGMEM = "STATUS:INIT:v1.0f";
const char MSG_READY[] PROGMEM = "STATUS:READY";
const char MSG_HOMING[] PROGMEM = "STATUS:HOMING";
const char MSG_HOMED[] PROGMEM = "STATUS:HOMED:0";
const char MSG_TARGET[] PROGMEM = "STATUS:MOVING";
const char MSG_AT_TARGET[] PROGMEM = "STATUS:TARGET_REACHED";
const char MSG_RETURNING[] PROGMEM = "STATUS:RETURNING";
const char MSG_AT_ORIGIN[] PROGMEM = "STATUS:ORIGIN_REACHED";
const char MSG_STOPPED[] PROGMEM = "STATUS:STOPPED";
const char MSG_ERROR[] PROGMEM = "ERROR:INVALID_STATE";

// === ENUMS ===
enum SystemState : uint8_t {
  STATE_IDLE,
  STATE_HOMING,
  STATE_HOMED,
  STATE_MOVING_TO_TARGET,
  STATE_AT_TARGET,
  STATE_RETURNING,
  STATE_AT_ORIGIN,
  STATE_ERROR
};

enum Direction : uint8_t {
  FORWARD = 0,
  BACKWARD = 1
};

// === COMPACT STEPPER CONTROLLER ===
class StepperController {
private:
  // State variables
  SystemState systemState_;
  long currentPosition_;
  long targetPosition_;
  long homePosition_;
  
  // Motion control
  unsigned long lastStepTime_;
  unsigned long currentInterval_;
  unsigned long targetInterval_;
  
  // Flags packed into single byte
  struct {
    uint8_t motorEnabled : 1;
    uint8_t direction : 1;  // 0=FORWARD, 1=BACKWARD
    uint8_t homingComplete : 1;
    uint8_t sensorTriggered : 1;
    uint8_t movingAwayFromSensor : 1;
    uint8_t reserved : 3;
  } flags_;
  
public:
  StepperController() :
    systemState_(STATE_IDLE),
    currentPosition_(0),
    targetPosition_(DEFAULT_TARGET_POSITION),
    homePosition_(0),
    lastStepTime_(micros()),
    currentInterval_(0),
    targetInterval_(0) {
    
    flags_.motorEnabled = 0;
    flags_.direction = 0;
    flags_.homingComplete = 0;
    flags_.sensorTriggered = 0;
    flags_.movingAwayFromSensor = 0;
  }
  
  void begin() {
    // Setup pins
    pinMode(STEP_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);
    pinMode(ENABLE_PIN, OUTPUT);
    pinMode(SENSOR_PIN, INPUT);
    
    digitalWrite(STEP_PIN, LOW);
    digitalWrite(DIR_PIN, LOW);
    digitalWrite(ENABLE_PIN, LOW);  // Start with motor disabled (LOW = disabled)
    
    // Calculate target interval for desired RPM
    float stepsPerSecond = (TARGET_RPM / 60.0f) * (FULL_STEPS_PER_REV * MICROSTEPS);
    targetInterval_ = 1000000UL / (unsigned long)stepsPerSecond;
    currentInterval_ = targetInterval_ * HOMING_SPEED_DIVISOR;
    
    // Clean startup - wait for serial to stabilize
    delay(100);
    while(Serial.available()) Serial.read();  // Flush input buffer
    Serial.flush();  // Flush output buffer
    Serial.println();  // Clear any startup noise
    
    Serial.print(F("CONFIG:"));
    Serial.print(TARGET_RPM, 0);
    Serial.print(F(":"));
    Serial.print((long)FULL_STEPS_PER_REV * MICROSTEPS);
    Serial.print(F(":"));
    Serial.println(targetInterval_);
    
    printProgmem(MSG_INIT);
    Serial.println();
    printProgmem(MSG_READY);
    Serial.println();
  }
  
  void update() {
    // State machine update
    updateStateMachine();
    
    // Motion update
    if (flags_.motorEnabled) {
      unsigned long currentTime = micros();
      if ((currentTime - lastStepTime_) >= currentInterval_) {
        performStep();
        lastStepTime_ = currentTime;
      }
    }
    // Removed debug output to reduce clutter
  }
  
  void startHoming() {
    // Always reset position before homing to allow backward movement
    currentPosition_ = 1000;  // Set to positive value so we can move backward
    
    if (systemState_ == STATE_IDLE || systemState_ == STATE_ERROR || systemState_ == STATE_HOMED) {
      systemState_ = STATE_HOMING;
      flags_.homingComplete = 0;
      flags_.sensorTriggered = 0;
      flags_.movingAwayFromSensor = 0;
      enableMotor(true);
      
      // Check if sensor is already triggered
      if (digitalRead(SENSOR_PIN)) {
        // Move away from sensor first (FORWARD = higher positions)
        setDirection(FORWARD);
        flags_.movingAwayFromSensor = 1;
        // Sensor active - moving away first (no serial output)
      } else {
        // Move toward sensor (BACKWARD = toward position 0)
        setDirection(BACKWARD);
        flags_.movingAwayFromSensor = 0;
        // Moving toward sensor (no serial output)
      }
      
      currentInterval_ = targetInterval_;  // Use normal speed for homing
      printProgmem(MSG_HOMING);
      Serial.println();
    }
  }
  
  void moveToTarget() {
    if (systemState_ == STATE_HOMED || systemState_ == STATE_AT_ORIGIN) {
      systemState_ = STATE_MOVING_TO_TARGET;
      enableMotor(true);
      setDirection(FORWARD); // Forward - away from home to compress syringe
      currentInterval_ = targetInterval_;  // Use normal speed
      printProgmem(MSG_TARGET);
      Serial.println();
    }
  }
  
  void returnToOrigin() {
    if (systemState_ == STATE_AT_TARGET) {
      systemState_ = STATE_RETURNING;
      enableMotor(true);
      setDirection(BACKWARD); // Backward - back to origin/home
      currentInterval_ = targetInterval_;  // Use normal speed
      printProgmem(MSG_RETURNING);
      Serial.println();
    }
  }
  
  void moveToPosition(long position) {
    if (systemState_ == STATE_HOMED || systemState_ == STATE_AT_TARGET || 
        systemState_ == STATE_AT_ORIGIN || systemState_ == STATE_IDLE) {
      if (position < MIN_POSITION || position > MAX_POSITION) {
        Serial.print(F("Invalid position ("));
        Serial.print(MIN_POSITION);
        Serial.print(F("-"));
        Serial.print(MAX_POSITION);
        Serial.println(F(")"));
        return;
      }
      
      // Determine direction based on current position
      if (position > currentPosition_) {
        setDirection(FORWARD); // Move forward to higher position
        targetPosition_ = position;
      } else if (position < currentPosition_) {
        setDirection(BACKWARD); // Move backward to lower position
        targetPosition_ = position;
      } else {
        Serial.println(F("Already at position"));
        return;
      }
      
      systemState_ = STATE_MOVING_TO_TARGET;
      enableMotor(true);
      currentInterval_ = targetInterval_;  // Use normal speed
      lastStepTime_ = micros();  // Reset timer for proper stepping
      
      Serial.print(F("Moving to: "));
      Serial.print(position);
      Serial.print(F(" from: "));
      Serial.print(currentPosition_);
      Serial.print(F(" dir: "));
      Serial.print(flags_.direction == FORWARD ? F("FWD") : F("BWD"));
      Serial.print(F(" Target Int: "));
      Serial.print(targetInterval_);
      Serial.print(F(" Current Int: "));
      Serial.println(currentInterval_);
    } else {
      Serial.print(F("Cannot move - busy, state: "));
      printStateName();
      Serial.println();
    }
  }
  
  void runCycle() {
    switch(systemState_) {
      case STATE_IDLE:
        startHoming();
        break;
      case STATE_HOMED:
      case STATE_AT_ORIGIN:
        moveToTarget();
        break;
      default:
        break;
    }
  }
  
  void stop() {
    enableMotor(false);
    // Go to IDLE from any state when manually stopped
    if (systemState_ != STATE_HOMED && systemState_ != STATE_AT_TARGET && 
        systemState_ != STATE_AT_ORIGIN) {
      systemState_ = STATE_IDLE;
      Serial.println(F("Stopped - returning to IDLE"));
    }
    printProgmem(MSG_STOPPED);
    Serial.println();
  }
  
  void setTargetPosition(long position) {
    if (position > MIN_POSITION && position < MAX_POSITION) {
      targetPosition_ = position;
      Serial.print(F("Target: "));
      Serial.println(position);
    }
  }
  
  long getPosition() const { return currentPosition_; }
  
  void resetSystem() {
    enableMotor(false);
    systemState_ = STATE_IDLE;
    currentPosition_ = 0;
    currentInterval_ = targetInterval_;  // Reset speed to prevent noise
    Serial.println(F("STATUS:RESET:0"));
  }
  
  void printStatus() const {
    Serial.print(F("STATUS:"));
    printStateName();
    Serial.print(F(":"));
    Serial.print(currentPosition_);
    Serial.print(F(":"));
    Serial.println(flags_.motorEnabled ? F("ON") : F("OFF"));
  }
  
private:
  void updateStateMachine() {
    switch(systemState_) {
      case STATE_HOMING:
        if (flags_.movingAwayFromSensor) {
          // Moving away from sensor - wait until sensor is clear
          if (!digitalRead(SENSOR_PIN)) {
            // Sensor cleared - now move back toward it
            setDirection(BACKWARD);
            flags_.movingAwayFromSensor = 0;
            // Sensor cleared - moving toward home (no serial output)
          }
        } else {
          // Moving toward sensor - wait for trigger
          boolean sensorActive = digitalRead(SENSOR_PIN);
          
          // NO SERIAL OUTPUT DURING HOMING MOVEMENT
          
          if (sensorActive) {
            // Sensor triggered - home found
            currentPosition_ = 0;
            homePosition_ = 0;
            systemState_ = STATE_HOMED;
            enableMotor(false);
            printProgmem(MSG_HOMED);
            Serial.println();
          }
        }
        break;
        
      case STATE_MOVING_TO_TARGET:
        // Check if reached target in either direction  
        if ((flags_.direction == FORWARD && currentPosition_ >= targetPosition_) ||  // Moving forward (increasing)
            (flags_.direction == BACKWARD && currentPosition_ <= targetPosition_)) {  // Moving backward (decreasing)
          systemState_ = STATE_AT_TARGET;
          enableMotor(false);
          printProgmem(MSG_AT_TARGET);
          Serial.println();
        }
        break;
        
      case STATE_RETURNING:
        if (currentPosition_ <= homePosition_) {
          systemState_ = STATE_AT_ORIGIN;
          enableMotor(false);
          printProgmem(MSG_AT_ORIGIN);
          Serial.println();
        }
        break;
        
      default:
        break;
    }
  }
  
  void performStep() {
    // SENSOR CHECK TEMPORARILY DISABLED - Sensor is damaged
    // Uncomment this block when sensor is replaced:
    /*
    if (flags_.direction == BACKWARD && digitalRead(SENSOR_PIN)) {
      if (systemState_ == STATE_HOMING && !flags_.movingAwayFromSensor) {
        // Homing: sensor found
        currentPosition_ = 0;
        homePosition_ = 0;
        systemState_ = STATE_HOMED;
        enableMotor(false);
        printProgmem(MSG_HOMED);
        Serial.println();
        return;
      } else {
        // Any other backward movement: emergency stop at sensor
        enableMotor(false);
        systemState_ = STATE_ERROR;
        Serial.println(F("ERROR: Sensor triggered"));
        return;
      }
    }
    */
    
    // Check position limits (but not during homing)
    if (flags_.direction == BACKWARD && currentPosition_ <= 0 && systemState_ != STATE_HOMING) {
      enableMotor(false);
      Serial.println(F("Position limit: Cannot go below 0"));
      return;
    }
    
    // NO DEBUG OUTPUT DURING MOVEMENT - causes timing issues
    
    // Generate step pulse
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(PULSE_WIDTH_US);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(PULSE_WIDTH_US);
    
    // Debug disabled - interferes with timing
    // static int debugStepCount = 0;
    // debugStepCount++;
    
    // Update position
    currentPosition_ += flags_.direction ? -1 : 1;
    
    // NO SERIAL OUTPUT DURING STEPPING
    
    // No acceleration - just use target speed
    currentInterval_ = targetInterval_;
  }
  
  void enableMotor(bool enable) {
    flags_.motorEnabled = enable;
    // YOUR TB6600 uses HIGH to enable, LOW to disable (same as version 1b)
    digitalWrite(ENABLE_PIN, enable ? HIGH : LOW);
  }
  
  void setDirection(Direction dir) {
    if (flags_.direction != dir) {
      flags_.direction = dir;
      delayMicroseconds(DIRECTION_SETUP_TIME_US);
    }
    // Always set the pin based on current direction
    // Hardware: HIGH moves away from sensor, LOW moves toward sensor
    digitalWrite(DIR_PIN, flags_.direction == FORWARD ? HIGH : LOW);
  }
  
  void printStateName() const {
    switch(systemState_) {
      case STATE_IDLE: Serial.print(F("IDLE")); break;
      case STATE_HOMING: Serial.print(F("HOMING")); break;
      case STATE_HOMED: Serial.print(F("HOMED")); break;
      case STATE_MOVING_TO_TARGET: Serial.print(F("TO_TARGET")); break;
      case STATE_AT_TARGET: Serial.print(F("AT_TARGET")); break;
      case STATE_RETURNING: Serial.print(F("RETURNING")); break;
      case STATE_AT_ORIGIN: Serial.print(F("AT_ORIGIN")); break;
      case STATE_ERROR: Serial.print(F("ERROR")); break;
    }
  }
  
  void printProgmem(const char* msg) const {
    char buffer[30];
    strcpy_P(buffer, msg);
    Serial.print(buffer);
  }
};

// === COMMAND PROCESSOR ===
class CommandProcessor {
private:
  StepperController& controller_;
  char commandBuffer_[20];
  uint8_t bufferIndex_;
  
public:
  CommandProcessor(StepperController& controller) : 
    controller_(controller),
    bufferIndex_(0) {}
  
  void processSerial() {
    while (Serial.available()) {
      char c = Serial.read();
      
      if (c == '\n' || c == '\r') {
        if (bufferIndex_ > 0) {
          commandBuffer_[bufferIndex_] = '\0';
          executeCommand();
          bufferIndex_ = 0;
        }
      } else if (bufferIndex_ < sizeof(commandBuffer_) - 1) {
        commandBuffer_[bufferIndex_++] = toupper(c);
      }
    }
  }
  
private:
  void executeCommand() {
    if (strcmp(commandBuffer_, "HOME") == 0) {
      controller_.startHoming();
    } else if (strcmp(commandBuffer_, "TARGET") == 0) {
      controller_.moveToTarget();
    } else if (strcmp(commandBuffer_, "RETURN") == 0) {
      controller_.returnToOrigin();
    } else if (strcmp(commandBuffer_, "CYCLE") == 0) {
      controller_.runCycle();
    } else if (strcmp(commandBuffer_, "STOP") == 0) {
      controller_.stop();
    } else if (strcmp(commandBuffer_, "STATUS") == 0) {
      controller_.printStatus();
    } else if (strcmp(commandBuffer_, "POS") == 0) {
      Serial.print(F("Position: "));
      Serial.println(controller_.getPosition());
    } else if (strncmp(commandBuffer_, "SET ", 4) == 0) {
      long value = atol(commandBuffer_ + 4);
      controller_.setTargetPosition(value);
    } else if (strncmp(commandBuffer_, "GOTO ", 5) == 0) {
      long position = atol(commandBuffer_ + 5);
      controller_.moveToPosition(position);
    } else if (strcmp(commandBuffer_, "RESET") == 0) {
      controller_.resetSystem();
    } else if (strcmp(commandBuffer_, "HELP") == 0) {
      printHelp();
    } else {
      Serial.println(F("ERROR:UNKNOWN_COMMAND"));
    }
  }
  
  void printHelp() {
    Serial.println(F("COMMANDS:HOME,TARGET,RETURN,CYCLE,GOTO,SET,STOP,RESET,STATUS,POS,HELP"));
  }
};

// === MAIN APPLICATION ===
StepperController stepper;
CommandProcessor commands(stepper);

void setup() {
  Serial.begin(115200);
  while (!Serial) { ; }
  
  stepper.begin();
  // M2M mode - no help prompts
}

void loop() {
  stepper.update();
  commands.processSerial();
}