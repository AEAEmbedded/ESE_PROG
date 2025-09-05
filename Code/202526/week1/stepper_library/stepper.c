/*
 * Stepper Motor Library for ATmega328P
 * Implementation file
 * 
 * Low level embedded code
 * v1.0
 * Aug 2025
 * Embedded Programming (Prog 5/6)
 */

#include "stepper.h"
#include <util/delay.h>

// Initialize stepper motor pins and settings
void stepper_init(void) {
    // Set pins as outputs
    DDRD |= (STEP_MASK | DIR_MASK | EN_MASK);
    
    // Set initial pin states
    PORTD &= ~STEP_MASK;  // STEP low
    PORTD &= ~DIR_MASK;   // DIR low (forward)
    PORTD |= EN_MASK;     // EN high (enable driver)
    
    // Small delay for driver initialization
    _delay_ms(10);
}

// Enable or disable the stepper driver
void stepper_enable(bool enable) {
    if (enable) {
        PORTD |= EN_MASK;   // EN high (enable)
    } else {
        PORTD &= ~EN_MASK;  // EN low (disable)
    }
    _delay_us(20);  // Setup time for TB6600
}

// Set stepper direction
void stepper_set_direction(stepper_direction_t direction) {
    if (direction == STEPPER_DIR_CW) {
        PORTD &= ~DIR_MASK;  // DIR low (clockwise)
    } else {
        PORTD |= DIR_MASK;   // DIR high (counter-clockwise)
    }
    _delay_us(20);  // Direction setup time for TB6600
}

// Generate a single step pulse
void stepper_step(void) {
    PORTD |= STEP_MASK;   // STEP high
    _delay_us(5);         // Minimum pulse width
    PORTD &= ~STEP_MASK;  // STEP low
}

// Move a specific number of steps with given delay
void stepper_move_steps(uint16_t steps, uint16_t delay_us) {
    for (uint16_t i = 0; i < steps; i++) {
        stepper_step();
        
        // Delay between steps (controls speed)
        // Split delay for delays > 255us (limit of _delay_us)
        if (delay_us > 255) {
            uint16_t ms = delay_us / 1000;
            uint16_t us = delay_us % 1000;
            for (uint16_t j = 0; j < ms; j++) {
                _delay_ms(1);
            }
            if (us > 255) {
                _delay_us(255);
                _delay_us(us - 255);
            } else if (us > 0) {
                _delay_us(us);
            }
        } else {
            _delay_us(delay_us);
        }
    }
}

// Move a specific number of revolutions at given RPM
void stepper_move_revolutions(uint8_t revolutions, uint16_t rpm) {
    // Calculate delay between steps for desired RPM
    // delay_us = (60 * 1000000) / (rpm * steps_per_rev)
    uint32_t delay_us = 60000000UL / ((uint32_t)rpm * STEPS_PER_REV);
    
    // Total steps for requested revolutions
    uint32_t total_steps = (uint32_t)revolutions * STEPS_PER_REV;
    
    // Move the motor
    for (uint32_t i = 0; i < total_steps; i++) {
        stepper_step();
        
        // Handle delays (same as in stepper_move_steps)
        if (delay_us > 255) {
            uint16_t ms = delay_us / 1000;
            uint16_t us = delay_us % 1000;
            for (uint16_t j = 0; j < ms; j++) {
                _delay_ms(1);
            }
            if (us > 255) {
                _delay_us(255);
                _delay_us(us - 255);
            } else if (us > 0) {
                _delay_us(us);
            }
        } else {
            _delay_us(delay_us);
        }
    }
}