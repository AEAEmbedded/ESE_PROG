/*
 * Arduino Stepper Motor Control - SOLID Architecture
 * State Machine for Syringe Control with Homing
 * Compatible with 8-bit 16MHz Arduino boards
 * 
 * v6.0 - Added state machine for syringe operation
 * - Homing sequence to find sensor zero point
 * - Target position for syringe compression
 * - Return to origin for syringe release
 * - Complete cycle automation
 * 
 * Sep 2025
 * Embedded Programming (Prog 5/6)
 * johan.korten@han.nl
 * SOLID Principles Applied with State Pattern
 */

// Forward declarations
class ILimit;
class ILimitResponse;
class StepperMotor;
class MotionProfile;
class MotionController;

// === LIMIT RESPONSE INTERFACE ===
// Defines what action to take when a limit is reached
class ILimitResponse {
public:
  enum Action {
    NONE,
    STOP,
    REVERSE,
    BACK_OFF
  };
  
  virtual ~ILimitResponse() {}
  virtual Action getAction() const = 0;
  virtual long getBackoffDistance() const { return 0; }
};

// === CONCRETE LIMIT RESPONSES ===
class StopResponse : public ILimitResponse {
public:
  Action getAction() const override { return STOP; }
};

class ReverseResponse : public ILimitResponse {
public:
  Action getAction() const override { return REVERSE; }
};

class BackOffResponse : public ILimitResponse {
private:
  long backoffDistance_;
public:
  BackOffResponse(long distance) : backoffDistance_(distance) {}
  Action getAction() const override { return BACK_OFF; }
  long getBackoffDistance() const override { return backoffDistance_; }
};

// === LIMIT INTERFACE ===
// Open/Closed Principle: Open for extension, closed for modification
class ILimit {
public:
  virtual ~ILimit() {}
  virtual boolean isReached(long position, int direction) const = 0;
  virtual ILimitResponse* getResponse() const = 0;
  virtual const char* getName() const = 0;
  virtual void reset() {}  // Optional reset for stateful limits
};

// === POSITION LIMIT ===
// Software limit based on absolute position
class PositionLimit : public ILimit {
private:
  long limitPosition_;
  int triggerDirection_;  // 1 for forward, -1 for reverse, 0 for both
  ILimitResponse* response_;
  const char* name_;
  
public:
  PositionLimit(long position, int direction, ILimitResponse* response, const char* name)
    : limitPosition_(position), 
      triggerDirection_(direction),
      response_(response),
      name_(name) {}
  
  boolean isReached(long position, int direction) const override {
    if (triggerDirection_ != 0 && triggerDirection_ != direction) {
      return false;
    }
    
    if (direction > 0) {  // Moving forward
      return position >= limitPosition_;
    } else {  // Moving reverse
      return position <= limitPosition_;
    }
  }
  
  ILimitResponse* getResponse() const override {
    return response_;
  }
  
  const char* getName() const override {
    return name_;
  }
};

// === SENSOR LIMIT ===
// Hardware limit using physical sensor
class SensorLimit : public ILimit {
private:
  uint8_t pin_;
  boolean activeHigh_;
  ILimitResponse* response_;
  const char* name_;
  
public:
  SensorLimit(uint8_t pin, boolean activeHigh, ILimitResponse* response, const char* name)
    : pin_(pin), 
      activeHigh_(activeHigh),
      response_(response),
      name_(name) {
    pinMode(pin_, INPUT);
  }
  
  boolean isReached(long position, int direction) const override {
    boolean sensorState = digitalRead(pin_);
    return activeHigh_ ? sensorState : !sensorState;
  }
  
  ILimitResponse* getResponse() const override {
    return response_;
  }
  
  const char* getName() const override {
    return name_;
  }
};

// === DISTANCE LIMIT ===
// Limit based on distance traveled from a reference point
class DistanceLimit : public ILimit {
private:
  long maxDistance_;
  long referencePosition_;
  ILimitResponse* response_;
  const char* name_;
  boolean active_;
  
public:
  DistanceLimit(long distance, ILimitResponse* response, const char* name)
    : maxDistance_(distance),
      referencePosition_(0),
      response_(response),
      name_(name),
      active_(true) {}
  
  boolean isReached(long position, int direction) const override {
    if (!active_) return false;
    long distance = abs(position - referencePosition_);
    return distance >= maxDistance_;
  }
  
  ILimitResponse* getResponse() const override {
    return response_;
  }
  
  const char* getName() const override {
    return name_;
  }
  
  void reset() override {
    referencePosition_ = 0;  // Will be set by controller
    active_ = true;
  }
  
  void setReference(long position) {
    referencePosition_ = position;
  }
  
  void setActive(boolean state) {
    active_ = state;
  }
};

// === LIMIT MANAGER ===
// Manages multiple limits and determines appropriate response
class LimitManager {
private:
  static const int MAX_LIMITS = 10;
  ILimit* limits_[MAX_LIMITS];
  int limitCount_;
  
public:
  LimitManager() : limitCount_(0) {
    for (int i = 0; i < MAX_LIMITS; i++) {
      limits_[i] = nullptr;
    }
  }
  
  void addLimit(ILimit* limit) {
    if (limitCount_ < MAX_LIMITS && limit != nullptr) {
      limits_[limitCount_++] = limit;
    }
  }
  
  void removeAllLimits() {
    limitCount_ = 0;
  }
  
  ILimit* checkLimits(long position, int direction) {
    for (int i = 0; i < limitCount_; i++) {
      if (limits_[i] && limits_[i]->isReached(position, direction)) {
        return limits_[i];
      }
    }
    return nullptr;
  }
  
  void resetAll() {
    for (int i = 0; i < limitCount_; i++) {
      if (limits_[i]) {
        limits_[i]->reset();
      }
    }
  }
};

// === STEPPER MOTOR CLASS ===
// Single Responsibility: Control physical stepper motor
class StepperMotor {
public:
  enum Direction {
    FORWARD = 1,
    REVERSE = -1,
    STOPPED = 0
  };

private:
  const uint8_t stepPin_;
  const uint8_t dirPin_;
  const uint8_t enablePin_;
  const long stepsPerRevolution_;
  const unsigned long minStepInterval_;
  
  long currentPosition_;
  Direction currentDirection_;
  boolean enabled_;
  
  static const uint8_t PULSE_WIDTH_US = 5;
  static const uint8_t DIRECTION_SETUP_TIME_US = 20;

public:
  StepperMotor(uint8_t stepPin, uint8_t dirPin, uint8_t enablePin,
               int fullStepsPerRev, int microsteps)
    : stepPin_(stepPin),
      dirPin_(dirPin),
      enablePin_(enablePin),
      stepsPerRevolution_((long)fullStepsPerRev * microsteps),
      minStepInterval_(calculateMinInterval(120.0)),
      currentPosition_(0),
      currentDirection_(STOPPED),
      enabled_(false) {}

  void begin() {
    pinMode(stepPin_, OUTPUT);
    pinMode(dirPin_, OUTPUT);
    pinMode(enablePin_, OUTPUT);
    
    digitalWrite(stepPin_, LOW);
    digitalWrite(dirPin_, LOW);
    digitalWrite(enablePin_, LOW);
  }

  void enable() {
    digitalWrite(enablePin_, HIGH);
    enabled_ = true;
  }

  void disable() {
    digitalWrite(enablePin_, LOW);
    enabled_ = false;
    currentDirection_ = STOPPED;
  }

  boolean isEnabled() const { return enabled_; }

  void step() {
    if (!enabled_) return;
    generatePulse();
    updatePosition();
  }

  void setDirection(Direction dir) {
    if (dir == STOPPED) return;
    
    if (dir != currentDirection_) {
      currentDirection_ = dir;
      digitalWrite(dirPin_, dir == REVERSE ? HIGH : LOW);
      delayMicroseconds(DIRECTION_SETUP_TIME_US);
    }
  }

  Direction getDirection() const { return currentDirection_; }
  long getPosition() const { return currentPosition_; }
  void setPosition(long pos) { currentPosition_ = pos; }
  void resetPosition() { currentPosition_ = 0; }
  long getStepsPerRevolution() const { return stepsPerRevolution_; }
  unsigned long getMinStepInterval() const { return minStepInterval_; }

private:
  void generatePulse() {
    digitalWrite(stepPin_, HIGH);
    delayMicroseconds(PULSE_WIDTH_US);
    digitalWrite(stepPin_, LOW);
    delayMicroseconds(PULSE_WIDTH_US);
  }

  void updatePosition() {
    currentPosition_ += currentDirection_;
  }

  unsigned long calculateMinInterval(float targetRPM) const {
    float stepsPerSecond = (targetRPM / 60.0f) * (float)stepsPerRevolution_;
    return 1000000UL / (unsigned long)stepsPerSecond;
  }
};

// === MOTION PROFILE CLASS ===
// Single Responsibility: Manage acceleration and speed profiles
class MotionProfile {
private:
  const unsigned long targetInterval_;
  const unsigned long accelStepSize_;
  const uint8_t decelMultiplier_;
  unsigned long currentInterval_;

public:
  MotionProfile(unsigned long targetInterval,
                unsigned long accelStepSize = 50,
                uint8_t decelMultiplier = 5)
    : targetInterval_(targetInterval),
      accelStepSize_(accelStepSize),
      decelMultiplier_(decelMultiplier),
      currentInterval_(targetInterval * 10) {}

  void accelerate() {
    if (currentInterval_ > targetInterval_) {
      currentInterval_ -= accelStepSize_;
      if (currentInterval_ < targetInterval_) {
        currentInterval_ = targetInterval_;
      }
    }
  }

  void resetForDirectionChange() {
    currentInterval_ = targetInterval_ * decelMultiplier_;
  }
  
  void resetSlow() {
    currentInterval_ = targetInterval_ * 10;
  }

  unsigned long getCurrentInterval() const {
    return currentInterval_;
  }
};

// === MOTION CONTROLLER CLASS ===
// Uses Dependency Injection for limits
class MotionController {
public:
  enum SystemState {
    IDLE,
    HOMING,
    HOMED,
    MOVING_TO_TARGET,
    AT_TARGET,
    RETURNING_TO_ORIGIN,
    AT_ORIGIN,
    ERROR
  };

private:
  StepperMotor& motor_;
  MotionProfile profile_;
  LimitManager& limitManager_;
  
  unsigned long lastStepTime_;
  long backoffTarget_;
  long backoffStart_;
  
  enum MotionState {
    RUNNING,
    BACKING_OFF,
    STOPPED
  };
  
  MotionState motionState_;
  SystemState systemState_;
  
  long homePosition_;
  long targetPosition_;
  long currentTargetPosition_;
  boolean homingComplete_;
  boolean sensorFoundDuringHoming_;

public:
  MotionController(StepperMotor& motor, LimitManager& limitManager)
    : motor_(motor),
      profile_(motor.getMinStepInterval()),
      limitManager_(limitManager),
      lastStepTime_(0),
      backoffTarget_(0),
      backoffStart_(0),
      motionState_(STOPPED),
      systemState_(IDLE),
      homePosition_(0),
      targetPosition_(3200),
      currentTargetPosition_(0),
      homingComplete_(false),
      sensorFoundDuringHoming_(false) {}

  void begin() {
    motor_.disable();
    motionState_ = STOPPED;
    systemState_ = IDLE;
    Serial.println("Motion controller initialized");
    Serial.println("System ready. Use HOME command to start homing sequence.");
  }

  void update() {
    updateStateMachine();
    
    if (!motor_.isEnabled() || motionState_ == STOPPED) {
      return;
    }

    unsigned long currentTime = micros();
    
    if (shouldStep(currentTime)) {
      if (motionState_ == BACKING_OFF) {
        handleBackoff();
      } else {
        handleNormalMovement();
      }
      lastStepTime_ = currentTime;
    }
  }
  
  void stop() {
    motor_.disable();
    motionState_ = STOPPED;
    if (systemState_ != IDLE && systemState_ != HOMED && systemState_ != AT_TARGET && systemState_ != AT_ORIGIN) {
      systemState_ = IDLE;
    }
    Serial.println("Motion stopped");
  }
  
  void startHoming() {
    if (systemState_ == IDLE || systemState_ == ERROR) {
      Serial.println("Starting homing sequence...");
      systemState_ = HOMING;
      homingComplete_ = false;
      sensorFoundDuringHoming_ = false;
      motor_.enable();
      motor_.setDirection(StepperMotor::REVERSE);
      motionState_ = RUNNING;
      profile_.resetSlow();
    } else {
      Serial.println("Cannot home - system not in IDLE state");
    }
  }
  
  void moveToTarget() {
    if (systemState_ == HOMED || systemState_ == AT_ORIGIN) {
      Serial.print("Moving to target position: ");
      Serial.println(targetPosition_);
      systemState_ = MOVING_TO_TARGET;
      currentTargetPosition_ = targetPosition_;
      motor_.enable();
      motor_.setDirection(StepperMotor::FORWARD);
      motionState_ = RUNNING;
      profile_.resetForDirectionChange();
    } else {
      Serial.println("Cannot move to target - not in HOMED or AT_ORIGIN state");
    }
  }
  
  void returnToOrigin() {
    if (systemState_ == AT_TARGET) {
      Serial.println("Returning to origin...");
      systemState_ = RETURNING_TO_ORIGIN;
      currentTargetPosition_ = homePosition_;
      motor_.enable();
      motor_.setDirection(StepperMotor::REVERSE);
      motionState_ = RUNNING;
      profile_.resetForDirectionChange();
    } else {
      Serial.println("Cannot return to origin - not at target position");
    }
  }
  
  void runCycle() {
    if (systemState_ == HOMED || systemState_ == AT_ORIGIN) {
      Serial.println("Running complete cycle: Home -> Target -> Origin");
      moveToTarget();
    } else if (systemState_ == IDLE) {
      Serial.println("System not homed. Starting homing first...");
      startHoming();
    } else {
      Serial.println("Cannot start cycle - system busy");
    }
  }
  
  void setTargetPosition(long position) {
    targetPosition_ = position;
    Serial.print("Target position set to: ");
    Serial.println(targetPosition_);
  }
  
  SystemState getSystemState() const { return systemState_; }
  
  const char* getSystemStateString() const {
    switch(systemState_) {
      case IDLE: return "IDLE";
      case HOMING: return "HOMING";
      case HOMED: return "HOMED";
      case MOVING_TO_TARGET: return "MOVING_TO_TARGET";
      case AT_TARGET: return "AT_TARGET";
      case RETURNING_TO_ORIGIN: return "RETURNING_TO_ORIGIN";
      case AT_ORIGIN: return "AT_ORIGIN";
      case ERROR: return "ERROR";
      default: return "UNKNOWN";
    }
  }

private:
  void updateStateMachine() {
    switch(systemState_) {
      case HOMING:
        if (homingComplete_) {
          motor_.resetPosition();
          homePosition_ = 0;
          systemState_ = HOMED;
          motionState_ = STOPPED;
          motor_.disable();
          Serial.println("Homing complete! Position reset to 0.");
        }
        break;
        
      case MOVING_TO_TARGET:
        if (abs(motor_.getPosition() - currentTargetPosition_) <= 1) {
          systemState_ = AT_TARGET;
          motionState_ = STOPPED;
          motor_.disable();
          Serial.println("Reached target position!");
          Serial.println("Syringe compressed - air expelled");
        }
        break;
        
      case RETURNING_TO_ORIGIN:
        if (abs(motor_.getPosition() - currentTargetPosition_) <= 1) {
          systemState_ = AT_ORIGIN;
          motionState_ = STOPPED;
          motor_.disable();
          Serial.println("Returned to origin!");
          Serial.println("Syringe released - ready for next cycle");
        }
        break;
        
      default:
        break;
    }
  }
  
  boolean shouldStep(unsigned long currentTime) const {
    return (currentTime - lastStepTime_) >= profile_.getCurrentInterval();
  }
  
  void handleNormalMovement() {
    if (systemState_ == HOMING) {
      ILimit* triggeredLimit = limitManager_.checkLimits(
        motor_.getPosition(), 
        motor_.getDirection()
      );
      
      if (triggeredLimit && strcmp(triggeredLimit->getName(), "Sensor") == 0) {
        Serial.println("Sensor detected during homing");
        homingComplete_ = true;
        sensorFoundDuringHoming_ = true;
      } else {
        motor_.step();
        profile_.accelerate();
      }
    } else if (systemState_ == MOVING_TO_TARGET || systemState_ == RETURNING_TO_ORIGIN) {
      long distanceToTarget = abs(motor_.getPosition() - currentTargetPosition_);
      if (distanceToTarget > 1) {
        motor_.step();
        profile_.accelerate();
      }
    } else {
      ILimit* triggeredLimit = limitManager_.checkLimits(
        motor_.getPosition(), 
        motor_.getDirection()
      );
      
      if (triggeredLimit) {
        handleLimitReached(triggeredLimit);
      } else {
        motor_.step();
        profile_.accelerate();
      }
    }
  }
  
  void handleBackoff() {
    motor_.step();
    
    long distanceBacked = abs(motor_.getPosition() - backoffStart_);
    if (distanceBacked >= backoffTarget_) {
      motionState_ = RUNNING;
      Serial.println("Backoff complete, resuming normal operation");
      profile_.resetForDirectionChange();
      limitManager_.resetAll();
    }
  }
  
  void handleLimitReached(ILimit* limit) {
    if (systemState_ != HOMING) {
      Serial.print("Limit reached: ");
      Serial.println(limit->getName());
    }
    
    ILimitResponse* response = limit->getResponse();
    
    switch (response->getAction()) {
      case ILimitResponse::STOP:
        stop();
        break;
        
      case ILimitResponse::REVERSE:
        reverseDirection();
        limitManager_.resetAll();
        break;
        
      case ILimitResponse::BACK_OFF:
        startBackoff(response->getBackoffDistance());
        break;
        
      default:
        break;
    }
  }
  
  void reverseDirection() {
    StepperMotor::Direction newDir = 
      (motor_.getDirection() == StepperMotor::FORWARD) 
      ? StepperMotor::REVERSE 
      : StepperMotor::FORWARD;
    
    motor_.setDirection(newDir);
    profile_.resetForDirectionChange();
    
    Serial.print("Direction reversed to ");
    Serial.println(newDir == StepperMotor::FORWARD ? "FORWARD" : "REVERSE");
  }
  
  void startBackoff(long distance) {
    motionState_ = BACKING_OFF;
    backoffStart_ = motor_.getPosition();
    backoffTarget_ = distance;
    reverseDirection();
    
    Serial.print("Starting backoff for ");
    Serial.print(distance);
    Serial.println(" steps");
  }
};

// === APPLICATION CLASS ===
class StepperApplication {
private:
  // Pin configuration
  static const uint8_t STEP_PIN = 7;
  static const uint8_t DIR_PIN = 6;
  static const uint8_t ENABLE_PIN = 5;
  static const uint8_t SENSOR_PIN = 12;
  
  // Motor configuration
  static const int FULL_STEPS_PER_REV = 200;
  static const int MICROSTEPS = 16;
  
  // Components
  StepperMotor motor_;
  LimitManager limitManager_;
  MotionController controller_;
  
  // Limit responses (owned by application)
  BackOffResponse sensorBackoff_;
  ReverseResponse travelReverse_;
  
  // Limits (owned by application)
  SensorLimit sensorLimit_;
  DistanceLimit travelLimit_;

public:
  StepperApplication()
    : motor_(STEP_PIN, DIR_PIN, ENABLE_PIN, FULL_STEPS_PER_REV, MICROSTEPS),
      limitManager_(),
      controller_(motor_, limitManager_),
      sensorBackoff_(200),  // 200 steps backoff
      travelReverse_(),
      sensorLimit_(SENSOR_PIN, true, &sensorBackoff_, "Sensor"),
      travelLimit_(3200, &travelReverse_, "Travel") {
    
    // Configure limits
    limitManager_.addLimit(&sensorLimit_);
    limitManager_.addLimit(&travelLimit_);
  }

  void begin() {
    Serial.begin(115200);
    Serial.println("=====================================");
    Serial.println("SOLID Architecture Stepper Control v6.0");
    Serial.println("=====================================");
    Serial.println("Syringe Control with State Machine");
    Serial.println("- Homing to sensor zero point");
    Serial.println("- Controlled compression/release");
    Serial.println();
    
    motor_.begin();
    controller_.begin();
    
    Serial.println();
    Serial.println("Type HELP for available commands");
  }

  void run() {
    controller_.update();
    processSerialCommands();
  }
  
private:
  void processSerialCommands() {
    if (Serial.available()) {
      String cmd = Serial.readStringUntil('\n');
      cmd.trim();
      cmd.toUpperCase();
      
      if (cmd == "STOP") {
        controller_.stop();
      } else if (cmd == "HOME") {
        controller_.startHoming();
      } else if (cmd == "TARGET") {
        controller_.moveToTarget();
      } else if (cmd == "RETURN") {
        controller_.returnToOrigin();
      } else if (cmd == "CYCLE") {
        controller_.runCycle();
      } else if (cmd.startsWith("SETTARGET ")) {
        String value = cmd.substring(10);
        long position = value.toInt();
        if (position > 0 && position < 10000) {
          controller_.setTargetPosition(position);
        } else {
          Serial.println("Invalid target position (must be 1-9999)");
        }
      } else if (cmd == "POS") {
        Serial.print("Position: ");
        Serial.println(motor_.getPosition());
      } else if (cmd == "STATUS") {
        printStatus();
      } else if (cmd == "HELP") {
        printHelp();
      } else {
        Serial.println("Unknown command. Type HELP for commands.");
      }
    }
  }
  
  void printStatus() {
    Serial.println("=== STATUS ===");
    Serial.print("System State: ");
    Serial.println(controller_.getSystemStateString());
    Serial.print("Position: ");
    Serial.println(motor_.getPosition());
    Serial.print("Direction: ");
    Serial.println(motor_.getDirection() == StepperMotor::FORWARD ? "FORWARD" : 
                   motor_.getDirection() == StepperMotor::REVERSE ? "REVERSE" : "STOPPED");
    Serial.print("Motor Enabled: ");
    Serial.println(motor_.isEnabled() ? "YES" : "NO");
  }
  
  void printHelp() {
    Serial.println("=== COMMANDS ===");
    Serial.println("HOME         - Start homing sequence (find sensor)");
    Serial.println("TARGET       - Move to target position (compress syringe)");
    Serial.println("RETURN       - Return to origin (release syringe)");
    Serial.println("CYCLE        - Run complete cycle (home->target->origin)");
    Serial.println("SETTARGET n  - Set target position (e.g., SETTARGET 3200)");
    Serial.println("STOP         - Emergency stop");
    Serial.println("POS          - Show current position");
    Serial.println("STATUS       - Show system status");
    Serial.println("HELP         - Show this help");
    Serial.println();
    Serial.println("=== OPERATION SEQUENCE ===");
    Serial.println("1. HOME - Initialize system and find zero point");
    Serial.println("2. TARGET - Compress syringe to expel air");
    Serial.println("3. RETURN - Release syringe back to origin");
    Serial.println("Or use CYCLE to run all steps automatically");
  }
};

// === MAIN PROGRAM ===
StepperApplication app;

void setup() {
  app.begin();
}

void loop() {
  app.run();
}