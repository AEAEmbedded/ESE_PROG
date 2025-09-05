/*
 * Example usage of Stepper Motor Library
 * ATmega328P
 * 
 * Low level embedded code
 * v1.0
 * Aug 2025
 * Embedded Programming (Prog 5/6)
 */

#include "stepper.h"
#include <util/delay.h>

int main(void) {
    // Initialize the stepper motor
    stepper_init();
    
    // Main loop
    while(1) {
        // Example 1: Move 1 revolution clockwise at 60 RPM
        stepper_set_direction(STEPPER_DIR_CW);
        stepper_move_revolutions(1, 60);
        _delay_ms(500);
        
        // Example 2: Move 1 revolution counter-clockwise at 120 RPM
        stepper_set_direction(STEPPER_DIR_CCW);
        stepper_move_revolutions(1, 120);
        _delay_ms(500);
        
        // Example 3: Move 1600 steps (half revolution) with custom timing
        stepper_set_direction(STEPPER_DIR_CW);
        stepper_move_steps(1600, 500);  // 500us between steps
        _delay_ms(500);
        
        // Example 4: Quick burst of steps
        for (int i = 0; i < 100; i++) {
            stepper_step();
            _delay_us(200);  // Fast stepping
        }
        _delay_ms(1000);
        
        // Example 5: Disable motor for 2 seconds (saves power)
        stepper_enable(false);
        _delay_ms(2000);
        stepper_enable(true);
        _delay_ms(100);
    }
    
    return 0;
}