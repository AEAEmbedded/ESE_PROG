# ECG Lead Detection Module - API Documentation

## Overview

The ECG Lead Detection module provides functionality for reading and transmitting ECG sensor data from three electrode positions (Left-Leg, Left-Arm, Right-Arm) via I2C communication. The module operates as an I2C slave device.

## Module Location

```
ECGLeadDetection/
├── FirmwareECGLeadDetect/    # Production firmware
└── BasicFirmware/            # Simplified test firmware
```

## Hardware Configuration

### I2C Configuration

| Parameter | Value |
|-----------|-------|
| I2C Address | `0x2A` (42 decimal) |
| Role | I2C Slave |
| Data Size | 6 bytes per request |

### Pin Assignments

| Sensor | Pin | Port | Wire Color |
|--------|-----|------|------------|
| Left-Leg (LL) | A1 | PB08 | Red |
| Left-Arm (LA) | A2 | PB09 | Black |
| Right-Arm (RA) | A3 | PA04 | White |
| Heartbeat LED | 14 | - | - |

### Serial Configuration

| Parameter | Value |
|-----------|-------|
| Baud Rate | 115200 |
| Debug Mode | Enabled via `TESTING` flag |

---

## Classes

### ECGSensor

Handles individual ECG sensor reading and value storage.

**Header:** `ECGSensor.h`

#### Constructor

```cpp
ECGSensor(uint8_t pin);
```

Creates an ECG sensor instance bound to a specific analog pin.

**Parameters:**
- `pin` - ADC pin number (e.g., A1, A2, A3)

**Example:**
```cpp
ECGSensor sensorLL(A1);  // Left-Leg sensor on pin A1
```

#### Methods

##### read()

```cpp
void read();
```

Reads the current analog value from the sensor pin. Stores a 16-bit value internally.

**Returns:** None

**Example:**
```cpp
sensorLL.read();
```

##### getValue()

```cpp
uint16_t getValue() const;
```

Returns the full 16-bit sensor reading.

**Returns:** `uint16_t` - The raw ADC value (0-65535)

**Example:**
```cpp
uint16_t rawValue = sensorLL.getValue();
```

##### getHighByte()

```cpp
uint8_t getHighByte() const;
```

Returns the high byte (bits 8-15) of the sensor value.

**Returns:** `uint8_t` - High byte of the reading

**Example:**
```cpp
uint8_t highByte = sensorLL.getHighByte();
```

##### getLowByte()

```cpp
uint8_t getLowByte() const;
```

Returns the low byte (bits 0-7) of the sensor value.

**Returns:** `uint8_t` - Low byte of the reading

**Example:**
```cpp
uint8_t lowByte = sensorLL.getLowByte();
```

---

### HeartBeat

Provides non-blocking LED heartbeat indication for system status.

**Header:** `HeartBeat.h`

#### Constructors

```cpp
HeartBeat();
HeartBeat(unsigned int ledPin, long interval);
```

**Default Constructor:**
- LED Pin: 14
- Interval: 100ms

**Custom Constructor Parameters:**
- `ledPin` - GPIO pin connected to the status LED
- `interval` - Blink interval in milliseconds

**Example:**
```cpp
HeartBeat hb;                    // Default: pin 14, 100ms
HeartBeat hb(LED_BUILTIN, 500);  // Custom: built-in LED, 500ms
```

#### Methods

##### begin()

```cpp
void begin();
```

Initializes the LED pin as OUTPUT. Call in `setup()`.

**Returns:** None

**Example:**
```cpp
void setup() {
    hb.begin();
}
```

##### blink()

```cpp
void blink();
```

Non-blocking LED toggle. Call repeatedly in `loop()` to maintain heartbeat.

**Returns:** None

**Example:**
```cpp
void loop() {
    hb.blink();
    // ... other code
}
```

---

## Functions

### ECGSensing Functions

**Header:** `ECGSensing.h`

#### readAndReportECGLeads()

```cpp
void readAndReportECGLeads();
```

Reads all three ECG sensors and outputs values via Serial.

**Output Format:**
```
ECG LL: <value>, LA: <value>, RA: <value>
```

---

### SerialHelper Functions

**Header:** `SerialHelper.h`

#### initSerial()

```cpp
void initSerial();
```

Initializes Serial communication at 115200 baud.

---

## I2C Data Protocol

### Request Event Response

When an I2C master requests data, the module responds with 6 bytes:

| Byte | Content | Description |
|------|---------|-------------|
| 0 | HLL | High byte of Left-Leg sensor |
| 1 | LLL | Low byte of Left-Leg sensor |
| 2 | HLA | High byte of Left-Arm sensor |
| 3 | LLA | Low byte of Left-Arm sensor |
| 4 | HRA | High byte of Right-Arm sensor |
| 5 | LRA | Low byte of Right-Arm sensor |

### Reading Data (Master Side)

```cpp
#include <Wire.h>

#define ECG_MODULE_ADDR 0x2A
#define NUM_SENSOR_BYTES 6

void readECGData() {
    Wire.requestFrom(ECG_MODULE_ADDR, NUM_SENSOR_BYTES);

    if (Wire.available() >= NUM_SENSOR_BYTES) {
        uint16_t ll = (Wire.read() << 8) | Wire.read();
        uint16_t la = (Wire.read() << 8) | Wire.read();
        uint16_t ra = (Wire.read() << 8) | Wire.read();

        // Process sensor values...
    }
}
```

---

## Constants

### ECGSensing.h

```cpp
#define SENSOR_LL A1  // Left-Leg sensor pin
#define SENSOR_LA A2  // Left-Arm sensor pin
#define SENSOR_RA A3  // Right-Arm sensor pin
```

### SerialHelper.h

```cpp
#define SERIAL_BAUDRATE 115200
```

### FirmwareECGLeadDetect.ino

```cpp
#define ECG_MODULE_ADDR 0x2A             // I2C slave address
#define HEARTBEAT_LEDPIN 14              // Status LED pin
#define DEFAULT_HEARTBEAT_INTERVAL 1000  // Heartbeat interval (ms)
#define TESTING 1                        // Enable serial debug output
#define NUM_SENSOR_BYTES 6               // Bytes per I2C response
```

---

## Complete Usage Example

### Slave Firmware (ECG Module)

```cpp
#include <Wire.h>
#include "ECGSensor.h"
#include "HeartBeat.h"

#define ECG_MODULE_ADDR 0x2A

ECGSensor sensorLL(A1);
ECGSensor sensorLA(A2);
ECGSensor sensorRA(A3);
HeartBeat hb(14, 1000);

void setup() {
    Serial.begin(115200);
    hb.begin();

    Wire.begin(ECG_MODULE_ADDR);
    Wire.onRequest(requestEvent);
}

void loop() {
    sensorLL.read();
    sensorLA.read();
    sensorRA.read();
    hb.blink();
}

void requestEvent() {
    Wire.write(sensorLL.getHighByte());
    Wire.write(sensorLL.getLowByte());
    Wire.write(sensorLA.getHighByte());
    Wire.write(sensorLA.getLowByte());
    Wire.write(sensorRA.getHighByte());
    Wire.write(sensorRA.getLowByte());
}
```

### Master Reading Example

```cpp
#include <Wire.h>

#define ECG_MODULE_ADDR 0x2A

void setup() {
    Wire.begin();
    Serial.begin(115200);
}

void loop() {
    Wire.requestFrom(ECG_MODULE_ADDR, 6);

    if (Wire.available() >= 6) {
        uint16_t ll = (Wire.read() << 8) | Wire.read();
        uint16_t la = (Wire.read() << 8) | Wire.read();
        uint16_t ra = (Wire.read() << 8) | Wire.read();

        Serial.print("LL: "); Serial.print(ll);
        Serial.print(" LA: "); Serial.print(la);
        Serial.print(" RA: "); Serial.println(ra);
    }

    delay(100);
}
```

---

## Dependencies

- Arduino.h (standard Arduino library)
- Wire.h (I2C communication)
