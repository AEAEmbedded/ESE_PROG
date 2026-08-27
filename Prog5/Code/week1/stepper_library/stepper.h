/*
 * Stepper Motor Library for ATmega328P
 * Header file
 * 
 * Low level embedded code
 * v1.0
 * Aug 2025
 * Embedded Programming (Prog 5/6)
 */

#ifndef STEPPER_H
#define STEPPER_H

#include <avr/io.h>
#include <stdbool.h>

// Pin definitions (ATmega328P)
#define STEP_PIN    7  // PD7 - TB6600 PUL
#define DIR_PIN     6  // PD6 - TB6600 DIR  
#define EN_PIN      5  // PD5 - TB6600 ENA

#define STEP_MASK   (1 << STEP_PIN)
#define DIR_MASK    (1 << DIR_PIN)
#define EN_MASK     (1 << EN_PIN)

// Motor parameters
#define FULL_STEPS_PER_REV  200
#define MICROSTEPS          16
#define STEPS_PER_REV       (FULL_STEPS_PER_REV * MICROSTEPS)

// Direction enum
typedef enum {
    STEPPER_DIR_CW = 0,   // Clockwise
    STEPPER_DIR_CCW = 1   // Counter-clockwise
} stepper_direction_t;

// Function prototypes
void stepper_init(void);
void stepper_enable(bool enable);
void stepper_set_direction(stepper_direction_t direction);
void stepper_step(void);
void stepper_move_steps(uint16_t steps, uint16_t delay_us);
void stepper_move_revolutions(uint8_t revolutions, uint16_t rpm);

#endif // STEPPER_H