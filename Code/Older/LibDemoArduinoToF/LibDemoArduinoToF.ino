#include "globalSettings.h"
#include <Wire.h>
#include "VL6180X.h"
#include "wiring_private.h"
#include "WireScanner.h"
#include "BlinkWithoutDelay.h"

// Heartbeat LED pin
#define ledHb 14
const unsigned long blinkInterval = 500;  // Blink every 500ms


// Define your I2C instances
TwoWire WireSensorA(&sercom1, W1_SDA, W1_SCL);
TwoWire WireSensorB(&sercom4, W2_SDA, W2_SCL);


// VL6180X instance
VL6180X* sensor = nullptr;

BlinkWithoutDelay heartBeat(ledHb, blinkInterval);


// Sensor configuration
struct SensorConfig {
  TwoWire* wire;
  uint8_t address;
  const char* label;
};

// Use the default address for both sensors
SensorConfig sensorConfigs[] = {
  { &WireSensorA, VL6180X::DEFAULT_SENSOR_ADDRESS, "Sensors A" },
  { &WireSensorB, VL6180X::DEFAULT_SENSOR_ADDRESS, "Sensors B" }
};

void setupSerial() {
  delay(1500);
  Serial.begin(115200);
  delay(1500);
}

void setupHeartbeatLED() {
  pinMode(ledHb, OUTPUT);
  digitalWrite(ledHb, HIGH);
}

void setupI2CBuses() {
  Serial.println("Initialize the I2C buses...");

  Wire.begin();
  WireSensorA.begin();
  WireSensorB.begin();

  setPinPeripheralAltStates(W1_SDA, W1_SCL);
  setPinPeripheralStates(W2_SDA, W2_SCL);
}

// Create a derived class to override the scanning behavior
class VL6180XScanner : public WireScanner {
public:
  VL6180XScanner(TwoWire* wire, const char* label)
    : WireScanner(wire, label) {}

  // Override scanForDevice to only look for the VL6180X address
  bool scanForDevice() override {
    return WireScanner::scanForDevice(VL6180X::DEFAULT_SENSOR_ADDRESS);
  }
};

void scanAndInitializeSensor() {
  for (int i = 0; i < 2 && sensor == nullptr; i++) {
    VL6180XScanner scanner(sensorConfigs[i].wire, sensorConfigs[i].label);
    if (scanner.scanForDevice(sensorConfigs[i].address)) {
      sensor = new VL6180X(sensorConfigs[i].address, sensorConfigs[i].wire);
      if (sensor->begin()) {
        Serial.print("VL6180X sensor initialized on ");
        Serial.println(sensorConfigs[i].label);
        sensor->init();
        sensor->configureDefault();
      } else {
        Serial.print("Sensor initialization failed on ");
        Serial.println(sensorConfigs[i].label);
        delete sensor;
        sensor = nullptr;
      }
    }
  }

  if (sensor == nullptr) {
    Serial.println("VL6180X sensor not found on either bus!");
  }
}

void setup() {
  setupSerial();
  setupHeartbeatLED();
  setupI2CBuses();
  digitalWrite(ledHb, LOW);
  scanAndInitializeSensor();
}

void loop() {
  heartBeat.update();

  if (sensor != nullptr) {
    uint8_t range = sensor->readRangeSingle();  // seems to have an issue...
    Serial.print("Range from sensor: ");
    Serial.print(range);
    Serial.println(" mm");
  }

}

void setPinPeripheralStates(uint8_t _dataPin, uint8_t _clockPin) {
  pinPeripheral(_dataPin, PIO_SERCOM);
  pinPeripheral(_clockPin, PIO_SERCOM);
}

void setPinPeripheralAltStates(uint8_t _dataPin, uint8_t _clockPin) {
  pinPeripheral(_dataPin, PIO_SERCOM_ALT);
  pinPeripheral(_clockPin, PIO_SERCOM_ALT);
}
