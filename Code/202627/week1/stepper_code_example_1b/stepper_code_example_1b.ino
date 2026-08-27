/*
 * AVR-C Stepper Motor Control for ATmega328P
 * Everything in main() - no functions
 * F_CPU = 16MHz assumed
 * 
 * Low level embedded code - structured C
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

#define ACCEL_STEP_SIZE     100
#define DECEL_MULTIPLIER    5

// --- Motion state variables ---
volatile long currentPosition = 0;
volatile long targetPosition = STEPS_PER_REV;
volatile uint16_t currentStepInterval = TARGET_STEP_TICKS * 10; // start slow

// --- Function prototypes ---
// Hardware initialization
void initializeHardware(void);
void initPorts(void);
void initTimer1(void);
void enableInterrupts(void);

// Motion control
void executeMotionControl(void);
uint8_t isTimeForNextStep(uint16_t currentTime, uint16_t *lastStepTime);
void executeStepSequence(uint16_t currentTime, uint16_t *lastStepTime);
void handleTargetReached(void);

// Position management  
void updatePosition(void);
uint8_t hasReachedTarget(void);
void reverseDirection(void);
long calculateNewTarget(void);

// Step generation
void makeStep(void);
void generateStepPulse(void);

// Direction control
void updateDirection(void);
void setDirectionForward(void);
void setDirectionReverse(void);
uint8_t isMovingForward(void);

// Speed/acceleration control
void handleAcceleration(void);
void accelerateMotor(void);
uint8_t canAccelerate(void);
void resetSpeedForDirectionChange(void);

// Timing utilities
void delayMicroseconds(uint16_t us);
uint16_t getTimer1(void);
uint16_t getTimeDifference(uint16_t current, uint16_t previous);

// System utilities
void initializeMotionParameters(void);
void performSystemSetup(void);

int main(void) {
    // Initialize all systems
    performSystemSetup();
    
    // Main control loop
    while(1) {
        executeMotionControl();
    }
    
    return 0;
}

// === SYSTEM SETUP FUNCTIONS ===

void performSystemSetup(void) {
    initializeHardware();
    initializeMotionParameters();
}

void initializeHardware(void) {
    initPorts();
    initTimer1();
    enableInterrupts();
}

void initPorts(void) {
    // Set pins as outputs (Data Direction Register)
    DDRD |= (STEP_MASK | DIR_MASK | EN_MASK);
    
    // Initialize pin states
    PORTD &= ~STEP_MASK;  // STEP low
    PORTD &= ~DIR_MASK;   // DIR low (forward)
    PORTD |= EN_MASK;     // EN high (enable driver)
}

void initTimer1(void) {
    // Timer1: 16-bit timer, prescaler = 8, normal mode
    // This gives us 0.5µs resolution at 16MHz
    TCCR1A = 0x00;  // Normal mode
    TCCR1B = 0x02;  // Prescaler = 8 (CS11 = 1)
    TCNT1 = 0;      // Reset counter
}

void enableInterrupts(void) {
    sei();
}

void initializeMotionParameters(void) {
    currentPosition = 0;
    targetPosition = STEPS_PER_REV;
    currentStepInterval = TARGET_STEP_TICKS * 10;
    updateDirection();
}

// === MAIN MOTION CONTROL ===

void executeMotionControl(void) {
    static uint16_t lastStepTime = 0;
    uint16_t currentTime = getTimer1();
    
    if (isTimeForNextStep(currentTime, &lastStepTime)) {
        if (hasReachedTarget()) {
            handleTargetReached();
        } else {
            executeStepSequence(currentTime, &lastStepTime);
        }
    }
}

uint8_t isTimeForNextStep(uint16_t currentTime, uint16_t *lastStepTime) {
    return (getTimeDifference(currentTime, *lastStepTime) >= currentStepInterval);
}

void executeStepSequence(uint16_t currentTime, uint16_t *lastStepTime) {
    makeStep();
    *lastStepTime = currentTime;
    updatePosition();
    handleAcceleration();
}

void handleTargetReached(void) {
    reverseDirection();
    resetSpeedForDirectionChange();
    updateDirection();
}

// === POSITION MANAGEMENT ===

void updatePosition(void) {
    if (isMovingForward()) {
        currentPosition++;
    } else {
        currentPosition--;
    }
}

uint8_t hasReachedTarget(void) {
    return (currentPosition == targetPosition);
}

void reverseDirection(void) {
    targetPosition = calculateNewTarget();
}

long calculateNewTarget(void) {
    return -currentPosition; // ping-pong motion
}

uint8_t isMovingForward(void) {
    return (targetPosition > currentPosition);
}

// === STEP GENERATION ===

void makeStep(void) {
    generateStepPulse();
}

void generateStepPulse(void) {
    // Generate step pulse - TB6600 steps on rising edge
    PORTD |= STEP_MASK;   // STEP high
    delayMicroseconds(5); // Minimum pulse width for TB6600
    PORTD &= ~STEP_MASK;  // STEP low
    delayMicroseconds(5); // Minimum low time
}

// === DIRECTION CONTROL ===

void updateDirection(void) {
    if (isMovingForward()) {
        setDirectionForward();
    } else {
        setDirectionReverse();
    }
    delayMicroseconds(20);   // Direction setup time for TB6600
}

void setDirectionForward(void) {
    PORTD &= ~DIR_MASK;  // DIR low = forward
}

void setDirectionReverse(void) {
    PORTD |= DIR_MASK;   // DIR high = reverse
}

// === SPEED/ACCELERATION CONTROL ===

void handleAcceleration(void) {
    if (canAccelerate()) {
        accelerateMotor();
    }
}

uint8_t canAccelerate(void) {
    return (currentStepInterval > TARGET_STEP_TICKS);
}

void accelerateMotor(void) {
    currentStepInterval -= ACCEL_STEP_SIZE;
    if (currentStepInterval < TARGET_STEP_TICKS) {
        currentStepInterval = TARGET_STEP_TICKS;
    }
}

void resetSpeedForDirectionChange(void) {
    currentStepInterval = TARGET_STEP_TICKS * DECEL_MULTIPLIER;
}

// === TIMING UTILITIES ===

void delayMicroseconds(uint16_t us) {
    // For small delays, use cycle counting
    // At 16MHz: 16 cycles = 1µs
    if (us == 0) return;
    
    // This is approximate - for exact timing, use timer-based delays
    while (us--) {
        _delay_us(1);
    }
}

uint16_t getTimer1(void) {
    // Return current Timer1 value (16-bit)
    // Each tick = 0.5µs at 16MHz with prescaler 8
    return TCNT1;
}

uint16_t getTimeDifference(uint16_t current, uint16_t previous) {
    // Handle 16-bit timer overflow correctly
    return (uint16_t)(current - previous);
}

/*
 * Compilation command:
 * avr-gcc -mmcu=atmega328p -DF_CPU=16000000UL -Os -o stepper.elf stepper.c
 * avr-objcopy -j .text -j .data -O ihex stepper.elf stepper.hex
 * 
 * Programming:
 * avrdude -c usbasp -p atmega328p -U flash:w:stepper.hex
 */