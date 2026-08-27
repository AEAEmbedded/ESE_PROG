/*
 * AVR-C Stepper Motor Control for ATmega328P - ESSENTIALS
 * Simplified version with only core functionality
 * F_CPU = 16MHz assumed
 * 
 * Low level embedded code
 * v1.0g - Essentials
 * Aug 2025
 * Embedded Programming (Prog 5/6)
 * johan.korten@han.nl
 * MIC2 Style
 */

#include <avr/io.h>
#include <util/delay.h>

// Pin definitions (ATmega328P)
#define STEP_PIN    7  // PD7 - TB6600 PUL
#define DIR_PIN     6  // PD6 - TB6600 DIR  
#define EN_PIN      5  // PD5 - TB6600 ENA

#define STEP_MASK   (1 << STEP_PIN)
#define DIR_MASK    (1 << DIR_PIN)
#define EN_MASK     (1 << EN_PIN)

// Motor parameters
#define STEPS_PER_REV  3200  // 200 full steps * 16 microsteps
#define STEP_DELAY_US  500   // Microseconds between steps (adjust for speed)

int main(void) {
    // Initialize pins as outputs
    DDRD |= (STEP_MASK | DIR_MASK | EN_MASK);
    
    // Set initial pin states
    PORTD &= ~STEP_MASK;  // STEP low
    PORTD &= ~DIR_MASK;   // DIR low (forward)
    PORTD |= EN_MASK;     // EN high (enable driver)
    
    // Main loop - continuous rotation
    while(1) {
        // Make one complete revolution
        for(int i = 0; i < STEPS_PER_REV; i++) {
            // Generate step pulse
            PORTD |= STEP_MASK;   // STEP high
            _delay_us(5);         // Minimum pulse width
            PORTD &= ~STEP_MASK;  // STEP low
            
            // Delay before next step (controls speed)
            _delay_us(STEP_DELAY_US);
        }
        
        // Pause between revolutions
        _delay_ms(1000);
        
        // Toggle direction for next revolution
        PORTD ^= DIR_MASK;    // XOR toggles the direction bit
        _delay_us(20);        // Direction setup time for TB6600
    }
    
    return 0;
}