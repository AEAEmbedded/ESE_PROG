// EXACT copy of what works in version 1b
#define STEP_PIN 7
#define DIR_PIN 6
#define ENABLE_PIN 5

void setup() {
  Serial.begin(115200);
  
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  
  // EXACTLY like version 1b
  digitalWrite(STEP_PIN, LOW);
  digitalWrite(DIR_PIN, LOW);   // LOW = forward
  digitalWrite(ENABLE_PIN, HIGH); // HIGH = enabled
  
  Serial.println("Starting - DIR=LOW (forward), ENABLE=HIGH");
}

void loop() {
  // Move AWAY from sensor (what we call forward)
  digitalWrite(DIR_PIN, HIGH);  // HIGH = away from sensor
  Serial.println("Moving AWAY from sensor (HIGH)...");
  for(int i = 0; i < 1000; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(500);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(500);
  }
  
  delay(1000);
  
  // Move TOWARD sensor (what we call backward)
  digitalWrite(DIR_PIN, LOW);  // LOW = toward sensor
  Serial.println("Moving TOWARD sensor (LOW)...");
  for(int i = 0; i < 1000; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(500);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(500);
  }
  
  delay(1000);
}