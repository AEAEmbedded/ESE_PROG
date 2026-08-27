/*
 * Simple TB6600 Stepper Test
 * Tests if the hardware is working
 */

#define STEP_PIN 7
#define DIR_PIN 6
#define ENABLE_PIN 5

void setup() {
  Serial.begin(115200);
  Serial.println("TB6600 Simple Test");
  
  // Setup pins
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  
  // Set initial states
  digitalWrite(STEP_PIN, LOW);
  digitalWrite(DIR_PIN, LOW);
  digitalWrite(ENABLE_PIN, LOW);  // LOW = enabled for TB6600
  
  Serial.println("Motor should be enabled");
  Serial.println("Starting to step in 2 seconds...");
  delay(2000);
}

void loop() {
  Serial.println("Stepping 200 steps forward...");
  
  // Step 200 steps
  for(int i = 0; i < 200; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(500);  // 500us HIGH
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(500);  // 500us LOW
    delay(2);  // 2ms between steps (slow for testing)
  }
  
  Serial.println("Waiting 1 second...");
  delay(1000);
  
  // Change direction
  digitalWrite(DIR_PIN, HIGH);
  Serial.println("Stepping 200 steps backward...");
  
  // Step 200 steps
  for(int i = 0; i < 200; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(500);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(500);
    delay(2);
  }
  
  // Change direction back
  digitalWrite(DIR_PIN, LOW);
  
  Serial.println("Waiting 1 second...");
  delay(1000);
}