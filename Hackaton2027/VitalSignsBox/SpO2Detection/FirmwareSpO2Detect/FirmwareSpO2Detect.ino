/*
    FirmwareSpO2Detect.ino

    SpO2 Sensor Connection Detection Firmware

    Detects if a SpO2 sensor is connected using a pull-up resistor detection method.
    The SPO2_CONNECTION_A2 pin has a 10k pull-up to 3.3V.
    When sensor is connected, it pulls the line low.

    Johan Korten
    for HAN ESE / WKZ Hackaton Challenge 2026

    V1.0 Jan 2026
*/

#include <Wire.h>
#include "HeartBeat.h"
#include "SerialHelper.h"
#include "SPO2Sensor.h"
#include "SPO2Sensing.h"

// I2C Configuration
#define SPO2_MODULE_ADDR 0x2B  // I2C slave address for SpO2 detection module

// Pin Configuration (defined in SPO2Sensor.h)
// SPO2_CONNECTION_A2 = A2  // Detection pin with 10k pull-up to 3.3V
// SPO2_LED_D12 = 12        // RED LED output pin
#define HEARTBEAT_LEDPIN 14
#define DEFAULT_HEARTBEAT_INTERVAL 1000

// Detection threshold (below this value = sensor connected)
// At 10-bit ADC: 1023 = 3.3V, so ~512 = 1.65V
// With 10k pull-up, connected sensor should pull well below this
#define DETECTION_THRESHOLD 512

// Debug mode
#define TESTING 1  // Set to 0 to disable serial output

// Response size: 1 byte status + 2 bytes raw ADC value + 1 byte LED state
#define NUM_RESPONSE_BYTES 4

// Create instances (using default pins: A2 for detection, D12 for LED)
SPO2Sensor spo2Sensor(SPO2_CONNECTION_A2, SPO2_LED_D12, DETECTION_THRESHOLD);
HeartBeat heartBeat = HeartBeat(HEARTBEAT_LEDPIN, DEFAULT_HEARTBEAT_INTERVAL);

void setup() {
    heartBeat.begin();
    initSerial();

    // Initialize SpO2 sensor detection
    spo2Sensor.begin();

    // Setup I2C slave
    Wire.begin(SPO2_MODULE_ADDR);
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);

    if (TESTING) {
        Serial.println("SpO2 Detection Module initialized");
        Serial.print("I2C Address: 0x");
        Serial.println(SPO2_MODULE_ADDR, HEX);
        Serial.print("Detection Pin: A2 (threshold: ");
        Serial.print(DETECTION_THRESHOLD);
        Serial.println(")");
        Serial.println("RED LED Pin: D12");
    }
}

void loop() {
    heartBeat.blink();

    // Update sensor detection state
    spo2Sensor.update();

    if (TESTING) {
        readAndReportSpO2Status();
        delay(500);  // Slow down serial output for readability
    }
}

// I2C receive event handler (for future use)
void receiveEvent(int howMany) {
    (void)howMany;  // Avoid compiler warning

    while (Wire.available() > 1) {
        char c = Wire.read();
        if (TESTING) Serial.print(c);
    }
    if (Wire.available()) {
        int x = Wire.read();
        if (TESTING) Serial.println(x);
    }
}

// I2C request event handler
// Sends 4 bytes: [status, rawHigh, rawLow, ledState]
void requestEvent() {
    uint8_t response[NUM_RESPONSE_BYTES];

    response[0] = spo2Sensor.getStatusByte();           // 1 = connected, 0 = disconnected
    response[1] = spo2Sensor.getHighByte();             // Raw ADC high byte
    response[2] = spo2Sensor.getLowByte();              // Raw ADC low byte
    response[3] = spo2Sensor.isLedOn() ? 1 : 0;         // LED state

    Wire.write(response, NUM_RESPONSE_BYTES);
}
