# Stepper Motor Control v1.0f

Memory-optimized Arduino stepper motor controller with state machine for syringe operation. Designed for Arduino Uno with TB6600 stepper driver and NEMA 23 motor.

## Hardware Setup

### Components Required
- Arduino Uno (or compatible)
- TB6600 Stepper Driver
- NEMA 23 Stepper Motor (23HS8430 tested)
- Limit sensor (for homing)
- 12-48V Power supply for motor

### Pin Connections

```
Arduino Pin → TB6600
Pin 5 → ENA-
Pin 6 → DIR-
Pin 7 → PUL-
5V → ENA+, DIR+, PUL+ (common anode)
GND → Signal Ground

Arduino Pin 12 → Limit Sensor
```

### TB6600 Configuration
- **Enable Logic**: HIGH = motor enabled, LOW = disabled
- **Direction Logic**: HIGH = forward, LOW = backward
- **Current Setting**: Match motor specifications via DIP switches
- **Microstepping**: Set to 8 microsteps (recommended)

## Motor Configuration

Current settings optimized for NEMA 23 motor:
- **Speed**: 180 RPM
- **Microsteps**: 8
- **Steps per revolution**: 1600 (200 × 8)
- **Position range**: 0 to 22000 steps

## Serial Commands

All commands are sent via Serial (115200 baud). Responses use structured M2M format.

### Movement Commands

| Command | Description | Example |
|---------|-------------|---------|
| `HOME` | Find sensor and establish zero position | `HOME` |
| `GOTO n` | Move to absolute position n | `GOTO 1000` |
| `TARGET` | Move to predefined target position | `TARGET` |
| `RETURN` | Return to origin from target position | `RETURN` |
| `CYCLE` | Run complete cycle (home→target→origin) | `CYCLE` |

### Configuration Commands

| Command | Description | Example |
|---------|-------------|---------|
| `SET n` | Set target position for TARGET command | `SET 3200` |
| `STOP` | Emergency stop motor | `STOP` |
| `RESET` | Reset system to idle state at position 0 | `RESET` |

### Status Commands

| Command | Description | Example |
|---------|-------------|---------|
| `STATUS` | Show current state, position, motor status | `STATUS` |
| `POS` | Show current position only | `POS` |
| `HELP` | List available commands | `HELP` |

## Response Format (M2M)

All responses use structured format for machine parsing:

### Status Messages
```
STATUS:READY                    - System ready
STATUS:HOMING                   - Homing in progress
STATUS:HOMED:0                  - Homed at position 0
STATUS:MOVING                   - Moving to position
STATUS:TARGET_REACHED           - Reached target position
STATUS:STOPPED                  - Motor stopped
STATUS:RESET:0                  - System reset to position 0
```

### Information Messages
```
CONFIG:180:1600:208             - RPM:Steps/rev:Interval(μs)
MOVE:0:1000:FWD                 - From:To:Direction
POSITION:1500                   - Current position
TARGET_SET:3200                 - Target position set
```

### Error Messages
```
ERROR:BUSY:HOMING               - Cannot execute, system busy
ERROR:INVALID_POSITION:0-22000  - Position out of range
ERROR:UNKNOWN_COMMAND           - Command not recognized
```

## State Machine

The system operates with the following states:

1. **IDLE** - Ready to accept commands
2. **HOMING** - Finding zero position via sensor
3. **HOMED** - Zero position established
4. **MOVING_TO_TARGET** - Moving to specified position
5. **AT_TARGET** - At target position (syringe compressed)
6. **RETURNING** - Returning to origin
7. **AT_ORIGIN** - Back at origin (syringe released)
8. **ERROR** - Error state

## Typical Usage Sequence

### Manual Operation
```
HOME                           → STATUS:HOMED:0
GOTO 5000                      → MOVE:0:5000:FWD → STATUS:TARGET_REACHED
GOTO 0                         → MOVE:5000:0:BWD → STATUS:TARGET_REACHED
```

### Automatic Syringe Cycle
```
CYCLE                          → Complete home→compress→release cycle
```

### Configuration
```
SET 4000                       → TARGET_SET:4000
TARGET                         → MOVE:0:4000:FWD
RETURN                         → MOVE:4000:0:BWD
```

## Safety Features

- **Position Limits**: Cannot move below 0 or above MAX_POSITION
- **State Validation**: Commands only accepted in valid states
- **Homing Override**: Position limits ignored during homing
- **Emergency Stop**: STOP command halts motor immediately

## Code Architecture

### Memory Optimization
- Removed virtual functions and inheritance from v1.0e
- PROGMEM strings to save RAM
- Bit-packed flags structure
- Simplified class hierarchy

### Key Classes
- `StepperController`: Main motor control and state machine
- `CommandProcessor`: Serial command parsing and execution

### Performance
- **Program Memory**: ~5KB (15% of Arduino Uno)
- **RAM Usage**: ~300 bytes (15% of Arduino Uno)
- **Step Timing**: Microsecond precision without serial interference

## Troubleshooting

### Motor Not Moving
- Check TB6600 power supply (12-48V)
- Verify pin connections
- Ensure motor current setting matches motor specs
- Check enable logic (HIGH = enabled)

### Wrong Direction
- Swap motor coil connections (A+ ↔ A- OR B+ ↔ B-)
- Or modify direction logic in code

### Timing Issues
- Avoid serial output during movement
- Check microstepping settings match code (8 microsteps)
- Reduce speed if motor stalls

### Sensor Issues
- Verify sensor connection to pin 12
- Check sensor logic (HIGH = triggered)
- Test with multimeter during homing

## Configuration Parameters

Located in the header defines:

```cpp
#define TARGET_RPM 180.0f        // Motor speed
#define MICROSTEPS 8             // Driver microstepping
#define MAX_POSITION 22000       // Maximum position limit
#define PULSE_WIDTH_US 10        // Step pulse width
```

## Version History

- **v1.0f**: Memory-optimized version with M2M communication
- **v1.0e**: Full SOLID architecture (memory intensive)  
- **v1.0a-d**: Development versions

## License

Educational use - HAN University of Applied Sciences
Embedded Programming Course (Prog 5/6)