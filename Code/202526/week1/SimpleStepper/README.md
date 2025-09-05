# SimpleStepper Arduino Library v2.0

A clean, lightweight Arduino library for controlling stepper motors using TB6600 drivers, built with SOLID principles and clean code practices.

## Features
- **Type-safe enums** for direction and motor state
- **Clean API** following Single Responsibility Principle
- **Configuration struct** for easy setup
- **Power management** with enable/disable states
- **Speed control** in RPM
- **Simple movement** methods: step(), move(), rotate()
- **Const-correct** design with immutable pin assignments

## Design Principles
This library follows clean code principles:
- **SRP (Single Responsibility)**: Each method has one clear purpose
- **DRY (Don't Repeat Yourself)**: Reusable code, no duplication
- **KISS (Keep It Simple)**: Simple, intuitive API
- **Type Safety**: Strong typing with enum classes
- **RAII**: Resource management in constructor

## Installation

### Arduino IDE Library Manager
1. Open Arduino IDE
2. Go to Sketch → Include Library → Manage Libraries
3. Search for "SimpleStepper"
4. Click Install

### Manual Installation
1. Download the SimpleStepper folder
2. Move it to your Arduino libraries folder:
   - Windows: `Documents\Arduino\libraries\`
   - macOS: `~/Documents/Arduino/libraries/`
   - Linux: `~/Arduino/libraries/`
3. Restart Arduino IDE

## Hardware Setup

### Wiring Options

The library supports both active-low and active-high configurations.

#### Option 1: Active-Low Configuration (Default)
```
Arduino    TB6600 Driver
-------    -------------
D7    →    PUL- (Step)
D6    →    DIR- (Direction)  
D5    →    ENA- (Enable)
+5V   →    PUL+/DIR+/ENA+ (Common positive)
GND   →    Arduino GND
```
- Logic LOW (0V) = Active/Enabled
- Logic HIGH (5V) = Inactive/Disabled

#### Option 2: Active-High Configuration
```
Arduino    TB6600 Driver
-------    -------------
D7    →    PUL+ (Step)
D6    →    DIR+ (Direction)  
D5    →    ENA+ (Enable)
GND   →    PUL-/DIR-/ENA- (Common ground)
```
- Logic HIGH (5V) = Active/Enabled
- Logic LOW (0V) = Inactive/Disabled

### TB6600 DIP Switch Settings
Configure microsteps and current on the TB6600 driver:
- SW1-3: Microstep resolution (typically set to 16)
- SW4-6: Motor current setting (match your motor specs)

## Quick Start

```cpp
#include <SimpleStepper.h>

// Create stepper object with pin configuration
SimpleStepper stepper(7, 6, 5);  // step, dir, enable pins

void setup() {
  // Option 1: Initialize with defaults
  stepper.begin();
  
  // Option 2: Initialize with custom configuration
  MotorConfig config;
  config.stepsPerRevolution = 200;
  config.microsteps = 16;
  config.rpm = 60;
  stepper.begin(config);
}

void loop() {
  // Rotate one revolution clockwise
  stepper.setDirection(Direction::CLOCKWISE);
  stepper.rotate(1.0);
  
  delay(1000);
  
  // Rotate one revolution counter-clockwise
  stepper.setDirection(Direction::COUNTER_CLOCKWISE);
  stepper.rotate(1.0);
  
  delay(1000);
}
```

## API Reference

### Constructor
```cpp
SimpleStepper(uint8_t stepPin, uint8_t dirPin, uint8_t enablePin)
```
Creates a new stepper motor object with immutable pin assignments.

### Configuration Structure
```cpp
struct MotorConfig {
  uint16_t stepsPerRevolution = 200;  // Motor steps per revolution
  uint8_t microsteps = 16;            // Microstep setting
  uint16_t rpm = 60;                   // Speed in RPM
  SignalLogic signalLogic = SignalLogic::ACTIVE_LOW;  // Signal type (ACTIVE_LOW or ACTIVE_HIGH)
};
```

### Initialization
```cpp
void begin(const MotorConfig& config = MotorConfig())
```
Initialize the motor with optional configuration.

### Motor State Control
```cpp
void setState(MotorState state)     // Set motor state (ENABLED/DISABLED)
MotorState getState() const          // Get current motor state
```

### Direction Control
```cpp
void setDirection(Direction dir)     // Set direction (CLOCKWISE/COUNTER_CLOCKWISE)
Direction getDirection() const       // Get current direction
```

### Movement Methods
```cpp
void step()                          // Single step pulse
void move(uint32_t steps)            // Move specified number of steps
void rotate(float revolutions)       // Rotate specified revolutions
```

### Speed Control
```cpp
void setRPM(uint16_t rpm)           // Set speed in RPM
uint16_t getRPM() const              // Get current RPM setting
```

## Enumerations

### Direction
```cpp
enum class Direction : uint8_t {
  CLOCKWISE = 0,
  COUNTER_CLOCKWISE = 1
};
```

### MotorState
```cpp
enum class MotorState : uint8_t {
  DISABLED = 0,
  ENABLED = 1
};
```

### SignalLogic
```cpp
enum class SignalLogic : uint8_t {
  ACTIVE_HIGH = 0,  // Traditional: HIGH = active
  ACTIVE_LOW = 1    // Inverted: LOW = active
};
```

## Examples

### Basic Movement
```cpp
// Simple rotation
stepper.setDirection(Direction::CLOCKWISE);
stepper.rotate(2.5);  // Rotate 2.5 revolutions

// Precise stepping
stepper.move(400);    // Move exactly 400 steps
```

### Speed Control
```cpp
// Change speed dynamically
stepper.setRPM(30);   // Slow speed
stepper.rotate(1);
stepper.setRPM(180);  // Fast speed
stepper.rotate(1);
```

### Power Management
```cpp
// Save power when motor is idle
stepper.rotate(5);
stepper.setState(MotorState::DISABLED);  // Disable holding torque
delay(10000);                            // Do other tasks
stepper.setState(MotorState::ENABLED);   // Re-enable before moving
```

### Custom Configuration
```cpp
// Active-Low wiring (PUL-, DIR-, ENA-)
MotorConfig config;
config.stepsPerRevolution = 400;  // High resolution motor
config.microsteps = 32;           // 32 microsteps
config.rpm = 100;                 // 100 RPM
config.signalLogic = SignalLogic::ACTIVE_LOW;

stepper.begin(config);

// Active-High wiring (PUL+, DIR+, ENA+)
MotorConfig configHigh;
configHigh.signalLogic = SignalLogic::ACTIVE_HIGH;
stepper.begin(configHigh);
```

### Pattern Movement
```cpp
// Create a square pattern
for (int i = 0; i < 4; i++) {
  stepper.rotate(0.25);  // 90-degree turn
  delay(250);
}

// Oscillation
for (int i = 0; i < 5; i++) {
  stepper.setDirection(Direction::CLOCKWISE);
  stepper.move(200);
  stepper.setDirection(Direction::COUNTER_CLOCKWISE);
  stepper.move(200);
}
```

## Clean Code Example
See `examples/BasicMotorControl/BasicMotorControl.ino` for a complete demonstration following clean code principles with:
- Separated demonstration functions (SRP)
- Meaningful constant definitions
- Clear function names
- Proper use of enums and structs

## Library Architecture

```
SimpleStepper/
├── library.properties       # Library metadata (v2.0.0)
├── keywords.txt            # Syntax highlighting rules
├── README.md              # Documentation
├── src/                   # Source files
│   ├── SimpleStepper.h    # Header with enums and class definition
│   └── SimpleStepper.cpp  # Implementation following SOLID principles
└── examples/              
    └── BasicMotorControl/
        └── BasicMotorControl.ino  # Clean example code
```

## Design Decisions

### Why Enum Classes?
- **Type safety**: Cannot accidentally pass wrong integer values
- **Scoped names**: Prevents namespace pollution
- **Self-documenting**: Clear intent in code

### Why Configuration Struct?
- **Grouped parameters**: Related settings stay together
- **Default values**: Sensible defaults provided
- **Extensibility**: Easy to add new settings without breaking API

### Why Const Members?
- **Immutability**: Pin assignments cannot change after construction
- **Thread safety**: Const members are inherently thread-safe
- **Clear intent**: Shows pins are hardware constants

## Performance Notes
- Uses blocking delays (no interrupts)
- Direct port manipulation for optimal performance
- Suitable for applications not requiring concurrent operations
- Maximum reliable speed depends on motor and driver specifications

## Troubleshooting

| Problem | Possible Causes | Solutions |
|---------|-----------------|-----------|
| Motor not moving | Wiring issues | Check all connections |
| | Power supply | Verify voltage and current |
| | Motor disabled | Call `setState(MotorState::ENABLED)` |
| | Wrong pins | Verify pin numbers in constructor |
| Motor vibrating | Speed too high | Reduce RPM |
| | Insufficient current | Adjust TB6600 current setting |
| | Acceleration too fast | Add gradual speed changes |
| Wrong direction | Direction setting | Check `setDirection()` calls |
| | Wiring reversed | Swap motor coil connections |

## Version History
- **v2.0.0** - Complete refactor with clean code principles
  - Added enum classes for type safety
  - Implemented configuration struct
  - Applied SOLID principles
  - Simplified API
- **v1.0.0** - Initial release

## License
Educational use - Created for Embedded Programming Course  
HAN University of Applied Sciences, 2025

## Contributing
This library is designed for educational purposes. Students are encouraged to:
- Study the clean code principles applied
- Extend functionality while maintaining code quality
- Submit improvements following the same design patterns

## Author
Johan Korten - johan.korten@han.nl  
Embedded Programming Course, HAN University