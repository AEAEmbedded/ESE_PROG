#include "WireScanner.h"
#include "TwiPinHelper.h"

// i2c system bus
#define W0_SCL 27  // PA22
#define W0_SDA 26  // PA23

#define W1_SCL 39  // PA13
#define W1_SDA 28  // PA12

#define W2_SCL 13  // PA17
#define W2_SDA 11  // PA16

TwiPinPair portBackbone(W0_SCL, W0_SDA);
TwiPinPair portSensorsA(W1_SCL, W1_SDA);
TwiPinPair portSensorsB(W2_SCL, W2_SDA);

#define ledHb 14

TwoWire WireBackbone(&sercom3, W0_SDA, W0_SCL);  // Main
TwoWire WireSensorA(&sercom1, W1_SDA, W1_SCL);   // Sensor A
TwoWire WireSensorB(&sercom4, W2_SDA, W2_SCL);   // Sensor B

WireScanner scannerBackbone(&WireBackbone, "Mainbus");
WireScanner scannerSensorA(&WireSensorA, "Sensors A");
WireScanner scannerSensorB(&WireSensorB, "Sensors B");

void setup() {
  delay(1500);
  Serial.begin(115200);
  delay(1500);

  Wire.begin();
  WireBackbone.begin();
  WireSensorA.begin();
  WireSensorB.begin();

  pinMode(ledHb, OUTPUT);
  digitalWrite(ledHb, HIGH);

  portBackbone.setPinPeripheralStates();
  portSensorsA.setPinPeripheralAltStates();
  portSensorsB.setPinPeripheralStates();

  while (!Serial)
    ;

  Serial.println("Ready...");
  delay(1500);
  digitalWrite(ledHb, LOW);

  // Create WireScanner instances
}

void loop() {
  digitalWrite(ledHb, HIGH);
  delay(500);
  // Scan the buses
  scannerBackbone.scan();
  scannerSensorA.scan();
  scannerSensorB.scan();
  digitalWrite(ledHb, LOW);
  delay(500);
}