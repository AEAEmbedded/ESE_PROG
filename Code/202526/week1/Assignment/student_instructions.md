# Stepper Motor Library - Student Implementation Guide

## Overview
This starter code provides a structured framework for controlling a stepper motor using a TB6600 driver. You need to implement several key functions to make the library functional.

## Files Structure
- `stepper.h` - Header file with function prototypes and definitions (COMPLETE)
- `stepper.c` - Implementation file with TODO functions (INCOMPLETE - YOUR TASK)
- `main.c` - Example usage (COMPLETE)

## What You Need to Implement

### 1. `stepper_init()` - Initialize the System
**Location:** `stepper.c` line ~45
**Task:** Set up GPIO pins and Timer1
```c
void stepper_init(void) {
    // Set STEP_PIN, DIR_PIN, and EN_PIN as outputs in DDRD
    // Initialize pin states: STEP=LOW, DIR=LOW, EN=HIGH  
    // Configure Timer1: Normal mode, prescaler=8
    // Enable global interrupts
    // Set default speed
}
```

### 2. `stepper_set_direction()` - Control Direction
**Location:** `stepper.c` line ~80
**Task:** Set the DIR pin and add required delay
```c
void stepper_set_direction(stepper_direction_t direction) {
    // Set or clear DIR_PIN in PORTD based on direction
    // Add ~20µs delay for TB6600 setup time
    // Update stepper_state.currentDirection
}
```

### 3. `stepper_move_to()` - Set Target Position  
**Location:** `stepper.c` line ~60
**Task:** Configure movement to absolute position
```c
void stepper_move_to(long position) {
    // Set stepper_state.targetPosition
    // Determine direction (CW if target > current, CCW otherwise)
    // Call stepper_set_direction()
    // Set isMoving = true
}
```

### 4. `stepper_rpm_to_ticks()` - Speed Calculation
**Location:** `stepper.c` line ~115
**Task:** Convert RPM to timer ticks
```c
uint16_t stepper_rpm_to_ticks(float rpm) {
    // Convert RPM to steps per second
    // Calculate microseconds per step
    // Convert to timer ticks using US_TO_TICKS macro
    // Return the result
}
```

### 5. `stepper_make_pulse()` - Generate Step Pulse
**Location:** `stepper.c` line ~130
**Task:** Create the step pulse for TB6600
```c
void stepper_make_pulse(void) {
    // Set STEP_PIN high in PORTD
    // Delay ~5µs (80 nop instructions at 16MHz)
    // Set STEP_PIN low in PORTD  
    // Delay another ~5µs
}
```

### 6. `stepper_update()` - Main Control Function ⭐ MOST IMPORTANT
**Location:** `stepper.c` line ~140
**Task:** Implement the main stepping logic
```c
void stepper_update(void) {
    // Read TCNT1 for current time
    // Check if enough time passed: (currentTime - lastStepTime) >= currentStepInterval
    // If ready and not at target position:
    //   - Call stepper_make_pulse()
    //   - Update lastStepTime
    //   - Update currentPosition (++ or -- based on direction)
    //   - Handle acceleration (gradually decrease currentStepInterval)
    // Check if target reached and update isMoving flag
}
```

## Key Concepts to Remember

### Register-Level Programming
- Use `DDRD` to set pin directions (1 = output, 0 = input)
- Use `PORTD` to set pin states (1 = HIGH, 0 = LOW)
- Use bit manipulation: `|=` to set bits, `&= ~` to clear bits
- Example: `PORTD |= (1 << STEP_PIN)` sets STEP_PIN high

### Timer1 Usage
- Timer1 runs at 2MHz (16MHz / prescaler 8)
- Each tick = 0.5µs
- Read current time with `TCNT1`
- Timer overflows every 32.768ms (16-bit)

### Timing Calculations
```c
// Convert RPM to steps per second
float steps_per_second = (rpm / 60.0) * STEPS_PER_REV;

// Convert to microseconds per step  
uint32_t step_interval_us = 1000000UL / (uint32_t)steps_per_second;

// Convert to timer ticks
uint16_t step_interval_ticks = US_TO_TICKS(step_interval_us);
```

### Acceleration Implementation
Start with a large step interval (slow speed) and gradually decrease it:
```c
if (currentStepInterval > targetStepInterval) {
    currentStepInterval -= acceleration_step;  // Increase speed
    if (currentStepInterval < targetStepInterval) {
        currentStepInterval = targetStepInterval;  // Limit to max speed
    }
}
```

## Testing Strategy

1. **Start Simple:** Implement just `stepper_init()` and verify pins are set correctly
2. **Test Direction:** Implement `stepper_set_direction()` and check DIR pin with multimeter
3. **Test Pulses:** Implement `stepper_make_pulse()` and verify with oscilloscope
4. **Add Movement:** Implement `stepper_move_to()` and `stepper_update()` for basic movement
5. **Add Speed Control:** Implement `stepper_rpm_to_ticks()` for speed control
6. **Debug:** Use LED on spare pin to indicate when stepping occurs

## Common Mistakes to Avoid

- Forgetting to set pins as outputs in `DDRD`
- Not adding direction setup delay (motor won't change direction properly)
- Timer overflow handling (use `uint16_t` arithmetic for time differences)
- Forgetting to update `lastStepTime` after each step
- Not checking `isEnabled` flag before stepping

## Bonus Challenges (Advanced Students)

1. **Smooth Acceleration:** Implement proper S-curve acceleration/deceleration
2. **Interrupt-Based Timing:** Use Timer1 Compare Match interrupt instead of polling
3. **Multiple Motors:** Extend library to control multiple steppers
4. **Encoder Feedback:** Add closed-loop position control with encoder
5. **Communication:** Add UART commands to control motor remotely

## Debug Tips

- Use `asm volatile("nop")` loops for precise timing delays
- Toggle a debug LED to visualize stepping frequency
- Use UART to print current position and status
- Check with oscilloscope: STEP pulses should be 5µs wide, properly timed

Good luck with your implementation! 🚀