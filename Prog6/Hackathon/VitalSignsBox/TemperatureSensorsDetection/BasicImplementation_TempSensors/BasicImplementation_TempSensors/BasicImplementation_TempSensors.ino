/*
    Johan Korten
    for HAN ESE, WKZ, Capgemini / GET Hackaton Challenge 2025

    V1.0 Jan 2025

    Based on Wire Slave Sender and Receiver by Nicholas Zambetti <http://www.zambetti.com> March 2006

    For your convenience we implemented:
    - Reading both channels of the MCP3426 (CH1+ and CH2+). This detects if a temperature sensor is connected.
    - Very basic implementation just as proof of concept. (ToDo: serious refactoring ;)).

*/

#include <Wire.h>
#include "WireScanner.h"
#include "TwiPinHelper.h"

// I2C System Bus Configuration
#define W1_SCL 39  // PA13
#define W1_SDA 28  // PA12
#define W2_SCL 13  // PA17
#define W2_SDA 11  // PA16

// Pin Setup for I2C Ports
TwiPinPair portSensorsA(W1_SCL, W1_SDA);
TwiPinPair portSensorsB(W2_SCL, W2_SDA);

TwoWire WireSensorA(&sercom1, W1_SDA, W1_SCL);  // Sensor A
TwoWire WireSensorB(&sercom4, W2_SDA, W2_SCL);  // Sensor B

#define LED_HB 14  // Heartbeat LED

// MCP3426A0 Default Address (A0 connected to GND)
#define MCP3426_ADDR 0x68

// Initialize I2C Scanner
WireScanner scannerSensorA(&WireSensorA, "Sensors A");
WireScanner scannerSensorB(&WireSensorB, "Sensors B");

// Function to Read MCP3426 Channel
float readMCP3426Channel(TwoWire* wire, uint8_t channel) {
  uint8_t configByte = 0x10;  // Start single-shot conversion, 16-bit resolution
  configByte |= (channel << 5);  // Set Channel (0: CH1+, 1: CH2+)
  configByte |= 0x00;  // Gain x1
  
  wire->beginTransmission(MCP3426_ADDR);
  wire->write(configByte);
  wire->endTransmission();
  delay(100);  // Wait for conversion (16-bit mode needs ~100ms)

  wire->requestFrom(MCP3426_ADDR, (uint8_t)3);  // 3 bytes: 2 data + 1 config
  if (wire->available() == 3) {
    uint16_t data = (wire->read() << 8) | wire->read();  // Combine two bytes
    wire->read();  // Read the config byte (not used here)

    if (data & 0x8000) {  // Check for negative value
      data = ~data + 1;
      return -((float)data / 32768.0 * 2.048);  // Scale for ±2.048V
    }
    return (float)data / 32768.0 * 2.048;  // Scale for 16-bit ADC
  }
  return NAN;  // Return Not-a-Number if read failed
}

void setup() {
  delay(1500);
  Serial.begin(115200);
  delay(1500);

  // Start I2C ports
  WireSensorA.begin();
  WireSensorB.begin();

  portSensorsA.setPinPeripheralAltStates();
  portSensorsB.setPinPeripheralStates();

  pinMode(LED_HB, OUTPUT);
  digitalWrite(LED_HB, LOW);

  Serial.println("MCP3426 Dual Sensor Reader Ready...");
  delay(1500);
}

void loop() {
  digitalWrite(LED_HB, HIGH);

  // Read CH1+ and CH2+ on Sensor A
  float sensorA_CH1 = readMCP3426Channel(&WireSensorA, 0);  // Channel 1
  float sensorA_CH2 = readMCP3426Channel(&WireSensorA, 1);  // Channel 2

  // Read CH1+ and CH2+ on Sensor B
  float sensorB_CH1 = readMCP3426Channel(&WireSensorB, 0);  // Channel 1
  float sensorB_CH2 = readMCP3426Channel(&WireSensorB, 1);  // Channel 2

  // Print results
  Serial.println("Sensor A:");
  Serial.print("  CH1+ Voltage: "); Serial.println(sensorA_CH1, 4);
  Serial.print("  CH2+ Voltage: "); Serial.println(sensorA_CH2, 4);

  Serial.println("Sensor B:");
  Serial.print("  CH1+ Voltage: "); Serial.println(sensorB_CH1, 4);
  Serial.print("  CH2+ Voltage: "); Serial.println(sensorB_CH2, 4);

  Serial.println("-------------------------------------");

  digitalWrite(LED_HB, LOW);
  delay(1000);  // 1-second delay between reads
}
