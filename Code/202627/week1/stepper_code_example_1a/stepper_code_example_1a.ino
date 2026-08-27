/*
 * AVR-C Stepper Motor Control for ATmega328P
 * Everything in main() - no functions
 * F_CPU = 16MHz assumed
 * 
 * Low level embedded code
 * v1.0
 * Aug 2025
 * Embedded Programming (Prog 5/6)
 * johan.korten@han.nl
 * MIC2 Style
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// --- Pin definitions (ATmega328P) ---
// Arduino Pin 7 = PD7, Arduino Pin 6 = PD6, Arduino Pin 5 = PD5
#define STEP_PIN    7  // PD7 - TB6600 PUL
#define DIR_PIN     6  // PD6 - TB6600 DIR  
#define EN_PIN      5  // PD5 - TB6600 ENA

#define STEP_MASK   (1 << STEP_PIN)
#define DIR_MASK    (1 << DIR_PIN)
#define EN_MASK     (1 << EN_PIN)

// --- Motor/driver parameters ---
#define FULL_STEPS_PER_REV  200
#define MICROSTEPS          16
#define STEPS_PER_REV       ((long)FULL_STEPS_PER_REV * MICROSTEPS)

// --- Speed control ---
#define TARGET_RPM      120.0
#define ACCEL_RPM_S     300.0

// Calculate timing (16MHz clock, Timer1 with prescaler 8 = 0.5µs resolution)
#define TIMER_PRESCALER     8
#define TIMER_FREQ          (F_CPU / TIMER_PRESCALER)  // 2MHz
#define US_TO_TICKS(us)     ((us) * 2)  // 0.5µs per tick

// Step timing calculations
#define TARGET_SPS          ((TARGET_RPM / 60.0) * STEPS_PER_REV)
#define TARGET_STEP_INTERVAL_US  (1000000UL / (unsigned long)TARGET_SPS)
#define TARGET_STEP_TICKS   US_TO_TICKS(TARGET_STEP_INTERVAL_US)

int main(void) {
    // --- Motion state variables ---
    long currentPosition = 0;
    long targetPosition = STEPS_PER_REV;  // start with 1 revolution forward
    uint16_t currentStepInterval = TARGET_STEP_TICKS * 10; // start slow for acceleration
    uint16_t lastStepTime = 0;
    uint16_t currentTime;
    uint16_t i; // for delay loops
    
    // --- Initialize Ports ---
    // Set pins as outputs (Data Direction Register)
    DDRD |= (STEP_MASK | DIR_MASK | EN_MASK);
    
    // Initialize pin states
    PORTD &= ~STEP_MASK;  // STEP low
    PORTD &= ~DIR_MASK;   // DIR low (forward direction)
    PORTD |= EN_MASK;     // EN high (enable driver)
    
    // --- Initialize Timer1 ---
    // Timer1: 16-bit timer, prescaler = 8, normal mode
    // This gives us 0.5µs resolution at 16MHz
    TCCR1A = 0x00;  // Normal mode
    TCCR1B = 0x02;  // Prescaler = 8 (CS11 = 1)
    TCNT1 = 0;      // Reset counter
    
    // Enable interrupts (if needed later)
    sei();
    
    // --- Initial Direction Setup ---
    if (targetPosition > currentPosition) {
        PORTD &= ~DIR_MASK;  // DIR low = forward
    } else {
        PORTD |= DIR_MASK;   // DIR high = reverse  
    }
    // Direction setup time for TB6600
    for (i = 0; i < 320; i++) {  // ~20µs at 16MHz (16 cycles per µs)
        asm volatile("nop");
    }
    
    // --- Main Loop ---
    while(1) {
        // Get current timer value (0.5µs per tick)
        currentTime = TCNT1;
        
        // Check if enough time has passed for next step
        if ((uint16_t)(currentTime - lastStepTime) >= currentStepInterval) {
            
            if (currentPosition != targetPosition) {
                // --- Make Step Pulse ---
                PORTD |= STEP_MASK;   // STEP high
                
                // 5µs delay (80 cycles at 16MHz)
                for (i = 0; i < 80; i++) {
                    asm volatile("nop");
                }
                
                PORTD &= ~STEP_MASK;  // STEP low
                
                // Another 5µs delay 
                for (i = 0; i < 80; i++) {
                    asm volatile("nop");
                }
                
                lastStepTime = currentTime;
                
                // --- Update Position ---
                if (targetPosition > currentPosition) {
                    currentPosition++;
                } else {
                    currentPosition--;
                }
                
                // --- Simple Acceleration ---
                // Gradually decrease step interval (increase speed)
                if (currentStepInterval > TARGET_STEP_TICKS) {
                    currentStepInterval -= 100; // rough acceleration
                    if (currentStepInterval < TARGET_STEP_TICKS) {
                        currentStepInterval = TARGET_STEP_TICKS;
                    }
                }
                
            } else {
                // --- Reached Target - Reverse Direction (Ping-Pong) ---
                targetPosition = -currentPosition;
                currentStepInterval = TARGET_STEP_TICKS * 5; // slow down for direction change
                
                // Update direction
                if (targetPosition > currentPosition) {
                    PORTD &= ~DIR_MASK;  // DIR low = forward
                } else {
                    PORTD |= DIR_MASK;   // DIR high = reverse  
                }
                
                // Direction setup time for TB6600 (~20µs)
                for (i = 0; i < 320; i++) {
                    asm volatile("nop");
                }
            }
        }
        
        // Optional: add a small delay to prevent excessive CPU usage
        // This is not strictly necessary but can help with stability
        for (i = 0; i < 10; i++) {
            asm volatile("nop");
        }
    }
    
    return 0;
}