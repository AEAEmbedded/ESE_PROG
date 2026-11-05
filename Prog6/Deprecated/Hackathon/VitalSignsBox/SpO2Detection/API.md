# SpO2 Sensor Detection Module - API Documentation

## Overview

The SpO2 Sensor Detection module detects whether a SpO2 sensor is physically connected using a pull-up resistor detection method. It does not measure actual SpO2 values - it only detects sensor presence. The module also provides control for a RED LED indicator.

## Module Location

```
SpO2Detection/
└── FirmwareSpO2Detect/    # Production firmware
    ├── SPO2Sensor.h/cpp   # Sensor detection class
    ├── SPO2Sensing.h      # Helper functions
    ├── HeartBeat.h/cpp    # Status LED indicator
    ├── SerialHelper.h     # Serial initialization
    └── FirmwareSpO2Detect.ino
```

## Hardware Configuration

### Detection Principle

The SPO2_CONNECTION_A2 pin has a 10k pull-up resistor to 3.3V:
- **Sensor disconnected**: Line reads high (~3.3V, ADC ~1023)
- **Sensor connected**: Sensor pulls line low (ADC < threshold)

### I2C Configuration

| Parameter | Value |
|-----------|-------|
| I2C Address | `0x2B` (43 decimal) |
| Role | I2C Slave |
| Data Size | 4 bytes per request |

### Pin Assignments

| Function | Pin | Port | Description |
|----------|-----|------|-------------|
| Connection Detect | A2 | PB09 | 10k pull-up to 3.3V |
| RED LED | D12 | PA19 | SpO2 sensor indicator LED |
| Heartbeat LED | 14 | - | Status indicator |

### Serial Configuration

| Parameter | Value |
|-----------|-------|
| Baud Rate | 115200 |
| Debug Mode | Enabled via `TESTING` flag |

---

## Classes

### SPO2Sensor

Handles SpO2 sensor connection detection and RED LED control.

**Header:** `SPO2Sensor.h`

#### Constructor

```cpp
SPO2Sensor(uint8_t pin = SPO2_CONNECTION_A2, uint8_t ledPin = SPO2_LED_D12, uint16_t threshold = 512);
```

Creates a SpO2 sensor detection instance.

**Parameters:**
- `pin` - Analog pin for detection (default: A2)
- `ledPin` - Digital pin for RED LED (default: D12)
- `threshold` - ADC threshold for detection (default: 512)

**Example:**
```cpp
SPO2Sensor spo2Sensor;  // Use defaults
SPO2Sensor spo2Sensor(A2, 12, 512);  // Explicit configuration
```

#### Methods

##### begin()

```cpp
void begin();
```

Initializes the detection pin as INPUT and LED pin as OUTPUT. Call in `setup()`.

**Example:**
```cpp
void setup() {
    spo2Sensor.begin();
}
```

##### update()

```cpp
void update();
```

Reads the detection pin and updates the connection state. Call regularly in `loop()`.

**Example:**
```cpp
void loop() {
    spo2Sensor.update();
}
```

##### isConnected()

```cpp
bool isConnected() const;
```

Returns whether a SpO2 sensor is connected.

**Returns:** `true` if sensor is connected (line pulled low), `false` otherwise

**Example:**
```cpp
if (spo2Sensor.isConnected()) {
    Serial.println("SpO2 sensor detected!");
}
```

##### getRawValue()

```cpp
uint16_t getRawValue() const;
```

Returns the raw ADC reading from the detection pin.

**Returns:** `uint16_t` - ADC value (0-1023 for 10-bit, 0-4095 for 12-bit)

##### getHighByte()

```cpp
uint8_t getHighByte() const;
```

Returns the high byte of the raw ADC value.

**Returns:** `uint8_t` - Bits 8-15 of the reading

##### getLowByte()

```cpp
uint8_t getLowByte() const;
```

Returns the low byte of the raw ADC value.

**Returns:** `uint8_t` - Bits 0-7 of the reading

##### getStatusByte()

```cpp
uint8_t getStatusByte() const;
```

Returns connection status as a single byte.

**Returns:** `1` if connected, `0` if disconnected

##### setThreshold()

```cpp
void setThreshold(uint16_t threshold);
```

Sets the ADC threshold for connection detection.

**Parameters:**
- `threshold` - ADC value below which sensor is considered connected

##### getThreshold()

```cpp
uint16_t getThreshold() const;
```

Returns the current detection threshold.

**Returns:** `uint16_t` - Current threshold value

---

### RED LED Control Methods

##### ledOn()

```cpp
void ledOn();
```

Turns the RED LED on.

**Example:**
```cpp
spo2Sensor.ledOn();
```

##### ledOff()

```cpp
void ledOff();
```

Turns the RED LED off.

**Example:**
```cpp
spo2Sensor.ledOff();
```

##### setLed()

```cpp
void setLed(bool state);
```

Sets the RED LED state.

**Parameters:**
- `state` - `true` = on, `false` = off

**Example:**
```cpp
spo2Sensor.setLed(spo2Sensor.isConnected());  // LED follows connection state
```

##### isLedOn()

```cpp
bool isLedOn() const;
```

Returns the current LED state.

**Returns:** `true` if LED is on, `false` if off

---

## Functions

### SPO2Sensing Functions

**Header:** `SPO2Sensing.h`

#### readAndReportSpO2Status()

```cpp
void readAndReportSpO2Status();
```

Outputs sensor status via Serial.

**Output Format:**
```
SpO2 Sensor: CONNECTED (raw: 123, LED: ON)
SpO2 Sensor: DISCONNECTED (raw: 1020, LED: OFF)
```

---

## I2C Data Protocol

### Request Event Response

When an I2C master requests data, the module responds with 4 bytes:

| Byte | Content | Description |
|------|---------|-------------|
| 0 | Status | 1 = connected, 0 = disconnected |
| 1 | Raw High | High byte of ADC reading |
| 2 | Raw Low | Low byte of ADC reading |
| 3 | LED State | 1 = LED on, 0 = LED off |

### Reading Data (Master Side)

```cpp
#include <Wire.h>

#define SPO2_MODULE_ADDR 0x2B
#define NUM_RESPONSE_BYTES 4

void readSpO2Status() {
    Wire.requestFrom(SPO2_MODULE_ADDR, NUM_RESPONSE_BYTES);

    if (Wire.available() >= NUM_RESPONSE_BYTES) {
        uint8_t status = Wire.read();
        uint16_t rawValue = (Wire.read() << 8) | Wire.read();
        uint8_t ledState = Wire.read();

        Serial.print("Connected: ");
        Serial.print(status ? "YES" : "NO");
        Serial.print(", Raw: ");
        Serial.print(rawValue);
        Serial.print(", LED: ");
        Serial.println(ledState ? "ON" : "OFF");
    }
}
```

---

## Constants

### SPO2Sensor.h

```cpp
#define SPO2_CONNECTION_A2 A2  // Detection pin
#define SPO2_LED_D12 12        // RED LED pin
```

### FirmwareSpO2Detect.ino

```cpp
#define SPO2_MODULE_ADDR 0x2B             // I2C slave address
#define HEARTBEAT_LEDPIN 14               // Status LED pin
#define DEFAULT_HEARTBEAT_INTERVAL 1000   // Heartbeat interval (ms)
#define DETECTION_THRESHOLD 512           // ADC threshold
#define TESTING 1                         // Enable serial debug output
#define NUM_RESPONSE_BYTES 4              // Bytes per I2C response
```

---

## Complete Usage Example

### Slave Firmware (SpO2 Module)

```cpp
#include <Wire.h>
#include "SPO2Sensor.h"
#include "HeartBeat.h"

#define SPO2_MODULE_ADDR 0x2B

SPO2Sensor spo2Sensor;
HeartBeat heartBeat(14, 1000);

void setup() {
    Serial.begin(115200);
    heartBeat.begin();
    spo2Sensor.begin();

    Wire.begin(SPO2_MODULE_ADDR);
    Wire.onRequest(requestEvent);
}

void loop() {
    heartBeat.blink();
    spo2Sensor.update();

    // Optional: Turn LED on when sensor connected
    spo2Sensor.setLed(spo2Sensor.isConnected());
}

void requestEvent() {
    uint8_t response[4];
    response[0] = spo2Sensor.getStatusByte();
    response[1] = spo2Sensor.getHighByte();
    response[2] = spo2Sensor.getLowByte();
    response[3] = spo2Sensor.isLedOn() ? 1 : 0;
    Wire.write(response, 4);
}
```

### Master Reading Example

```cpp
#include <Wire.h>

#define SPO2_MODULE_ADDR 0x2B

void setup() {
    Wire.begin();
    Serial.begin(115200);
}

void loop() {
    Wire.requestFrom(SPO2_MODULE_ADDR, 4);

    if (Wire.available() >= 4) {
        bool connected = Wire.read();
        uint16_t raw = (Wire.read() << 8) | Wire.read();
        bool ledOn = Wire.read();

        Serial.print("SpO2 Sensor: ");
        Serial.print(connected ? "CONNECTED" : "DISCONNECTED");
        Serial.print(" (raw: ");
        Serial.print(raw);
        Serial.println(")");
    }

    delay(500);
}
```

---

## Detection Threshold Calculation

The detection threshold determines when the sensor is considered connected.

### For 10-bit ADC (0-1023)

| Condition | ADC Value | Voltage |
|-----------|-----------|---------|
| Disconnected (pull-up) | ~1023 | ~3.3V |
| Connected (pulled low) | < 512 | < 1.65V |

### Recommended Threshold

- **Default:** 512 (50% of full scale)
- **Conservative:** 300-400 (clearer distinction)

```cpp
// Adjust threshold if needed
spo2Sensor.setThreshold(400);
```

---

## Dependencies

- Arduino.h (standard Arduino library)
- Wire.h (I2C communication)
