# Stepper Motor Linear Actuator Syringe System Requirements

## System Overview
**Purpose**: Command-line controlled automated syringe dispensing system with predefined volume selections for air dispensing

## Functional Requirements

### Primary Functions

#### 1. Select Dispensing Volume (CLI)
- User enters command with volume parameter: `dispense <volume>`
- Valid volumes: 12.25, 21.0, 24.5, 28.0, 36.75, 73.5 (ml)
- System validates volume against predefined list
- System displays error message for invalid volumes
- Command: `dispense 24.5` or `list-volumes` to see options

#### 2. Dispense Air
- System executes air dispensing after valid volume command
- Actuator advances predetermined distance based on selected volume
- System provides real-time status updates via CLI
- System displays completion message with actual volume dispensed

#### 3. Home System
- User executes home command: `home`
- System retracts actuator until home switch is triggered
- System displays homing progress and completion status
- System confirms home position established

#### 4. Volume Verification
- Volume sensor measures actual air volume dispensed
- System displays actual vs. target volume comparison
- System shows dispensing accuracy percentage
- Command: `status` shows last dispensing results

### Secondary Functions

#### 5. System Information Commands
- `list-volumes`: Display all available volume options
- `status`: Show current system state and last operation results
- `help`: Display all available commands and usage
- `version`: Show system version and calibration date

#### 6. Administrative Commands
- `calibrate <volume>`: Calibrate specific volume setting
- `add-volume <value>`: Add new volume option (admin only)
- `test <volume>`: Perform test dispensing without actual air flow

#### 7. Error Handling
- Invalid command syntax shows usage help
- Sensor errors display diagnostic messages
- System provides clear error codes and recovery instructions
- `emergency-stop` command immediately halts all operations

## CLI Command Examples

```bash
> dispense 24.5
Dispensing 24.5ml of air...
Actuator moving... [████████████] 100%
Volume sensor reading: 24.3ml
Accuracy: 99.2%
Air dispensing complete.

> home
Homing actuator...
Home switch triggered.
System homed successfully.

> list-volumes
Available volumes (ml): 12.25, 21.0, 24.5, 28.0, 36.75, 73.5

> status
System Status: Ready
Last operation: Dispensed 24.5ml air (actual: 24.3ml)
Home position: Established
Sensors: Normal
```

## Actors
- **CLI User**: Laboratory technician, student, researcher using command line
- **System Administrator**: Uses admin commands for calibration and setup
- **System**: Provides automated responses and status updates via CLI

## Non-Functional Requirements
- **Interface**: Text-based command line interface only
- **Response Time**: Commands execute within 2 seconds, air dispensing completes within 15 seconds
- **Feedback**: Real-time text status updates during operations
- **Error Messages**: Clear, actionable error messages in CLI
- **Help System**: Built-in command help and usage examples

## Use Case Categories for Students

**Suggest students organize their use cases into:**

1. **Normal CLI Operations** (dispense air command, home command, status queries)
2. **Information Commands** (list volumes, help, status)
3. **Administrative CLI** (calibrate, add volumes, test mode)
4. **Error Handling** (invalid commands, emergency stop, sensor errors)

## Assignment Instructions

This CLI-focused approach gives students experience with command-line system use cases while working with the air dispensing functionality. Students should create use case diagrams that cover all four categories above, clearly identifying actors, use cases, and relationships between them.