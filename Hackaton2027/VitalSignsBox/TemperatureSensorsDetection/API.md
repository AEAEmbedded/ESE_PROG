# Temperature Sensors Detection Module - API Documentation

## Overview

The Temperature Sensors Detection module reads analog temperature sensor data using MCP3426 16-bit ADC chips over dual I2C buses. The module supports up to 4 differential temperature sensor channels.

## Module Location

```
TemperatureSensorsDetection/
└── BasicImplementation_TempSensors/
    └── BasicImplementation_TempSensors/
        └── BasicImplementation_TempSensors.ino
```

## Hardware Configuration

### I2C Bus Configuration

The module uses two independent I2C buses to communicate with MCP3426 ADC chips.

| Bus | SDA Pin | SCL Pin | Port (SDA) | Port (SCL) |
|-----|---------|---------|------------|------------|
| Sensor Bus A | 28 | 39 | PA12 | PA13 |
| Sensor Bus B | 11 | 13 | PA16 | PA17 |

### ADC Configuration

| Parameter | Value |
|-----------|-------|
| ADC Chip | MCP3426 |
| I2C Address | `0x68` |
| Resolution | 16-bit |
| Channels per chip | 2 differential |
| Voltage Range | ±2.048V |
| Gain | 1x |
| Conversion Mode | Single-shot |

### Pin Assignments

| Pin | Port | Function |
|-----|------|----------|
| 28 | PA12 | I2C Bus A - SDA |
| 39 | PA13 | I2C Bus A - SCL |
| 11 | PA16 | I2C Bus B - SDA |
| 13 | PA17 | I2C Bus B - SCL |
| 14 | - | Heartbeat LED |

### Serial Configuration

| Parameter | Value |
|-----------|-------|
| Baud Rate | 115200 |

---

## Constants

### Pin Definitions

```cpp
// I2C Port A
#define W1_SCL 39  // PA13 - Clock line
#define W1_SDA 28  // PA12 - Data line

// I2C Port B
#define W2_SCL 13  // PA17 - Clock line
#define W2_SDA 11  // PA16 - Data line

// Status LED
#define LED_HB 14  // Heartbeat LED
```

### I2C Address

```cpp
#define MCP3426_ADDR 0x68  // MCP3426 ADC address (A0 to GND)
```

---

## Functions

### readMCP3426Channel()

```cpp
float readMCP3426Channel(TwoWire* wire, uint8_t channel);
```

Reads a voltage value from a specific channel of the MCP3426 ADC.

**Parameters:**
- `wire` - Pointer to the TwoWire I2C bus instance (`&WireSensorA` or `&WireSensorB`)
- `channel` - ADC channel to read:
  - `0` - Channel 1 (differential input CH1+ vs CH1-)
  - `1` - Channel 2 (differential input CH2+ vs CH2-)

**Returns:**
- `float` - Voltage in volts (range: -2.048V to +2.048V)
- `NAN` - If communication fails or data not ready

**Configuration Applied:**
- Single-shot conversion mode
- 16-bit resolution (240 SPS)
- Gain: 1x (PGA disabled)

**Example:**
```cpp
float voltage = readMCP3426Channel(&WireSensorA, 0);
if (!isnan(voltage)) {
    Serial.print("Channel 1 Voltage: ");
    Serial.println(voltage, 4);
}
```

---

## Data Structures

### TwoWire Instances

```cpp
TwoWire WireSensorA(&sercom1, W1_SDA, W1_SCL);  // I2C Bus A
TwoWire WireSensorB(&sercom4, W2_SDA, W2_SCL);  // I2C Bus B
```

### TwiPinPair Instances

```cpp
TwiPinPair portSensorsA(W1_SCL, W1_SDA);  // Pin config for Bus A
TwiPinPair portSensorsB(W2_SCL, W2_SDA);  // Pin config for Bus B
```

### WireScanner Instances

```cpp
WireScanner scannerSensorA(&WireSensorA, "Sensors A");  // Scanner for Bus A
WireScanner scannerSensorB(&WireSensorB, "Sensors B");  // Scanner for Bus B
```

---

## MCP3426 ADC Protocol

### Configuration Byte Format

| Bit | Name | Value | Description |
|-----|------|-------|-------------|
| 7 | RDY | 1 | Start conversion (write) / Data ready (read) |
| 6-5 | C1-C0 | 00/01 | Channel select (00=CH1, 01=CH2) |
| 4 | O/C | 1 | Conversion mode (1=One-shot) |
| 3-2 | S1-S0 | 10 | Sample rate (10=16-bit, 15 SPS) |
| 1-0 | G1-G0 | 00 | PGA Gain (00=1x) |

### Configuration Values Used

```cpp
// Channel 1: 0b10001000 = 0x88
// - RDY=1 (start conversion)
// - Channel=00 (CH1)
// - O/C=1 (one-shot)
// - S1-S0=10 (16-bit)
// - G1-G0=00 (1x gain)

// Channel 2: 0b10101000 = 0xA8
// - RDY=1 (start conversion)
// - Channel=01 (CH2)
// - O/C=1 (one-shot)
// - S1-S0=10 (16-bit)
// - G1-G0=00 (1x gain)
```

### Data Format

The MCP3426 returns 3 bytes:
| Byte | Content |
|------|---------|
| 0 | Data High Byte |
| 1 | Data Low Byte |
| 2 | Configuration (RDY bit indicates conversion complete) |

### Voltage Calculation

```cpp
// For 16-bit, gain=1x:
// LSB = 2.048V / 32768 = 62.5µV
// Voltage = (int16_t)rawValue * (2.048 / 32768.0)
```

---

## Complete Usage Example

### Basic Temperature Reading

```cpp
#include <Wire.h>
#include "WireScanner.h"
#include "TwiPinHelper.h"

// Pin definitions
#define W1_SCL 39
#define W1_SDA 28
#define W2_SCL 13
#define W2_SDA 11
#define LED_HB 14
#define MCP3426_ADDR 0x68

// I2C bus instances
TwoWire WireSensorA(&sercom1, W1_SDA, W1_SCL);
TwoWire WireSensorB(&sercom4, W2_SDA, W2_SCL);

// Pin configuration helpers
TwiPinPair portSensorsA(W1_SCL, W1_SDA);
TwiPinPair portSensorsB(W2_SCL, W2_SDA);

float readMCP3426Channel(TwoWire* wire, uint8_t channel) {
    uint8_t config = 0x88 | (channel << 5);  // One-shot, 16-bit, gain=1

    // Start conversion
    wire->beginTransmission(MCP3426_ADDR);
    wire->write(config);
    wire->endTransmission();

    delay(100);  // Wait for conversion

    // Read result
    wire->requestFrom(MCP3426_ADDR, 3);
    if (wire->available() < 3) return NAN;

    uint8_t highByte = wire->read();
    uint8_t lowByte = wire->read();
    uint8_t cfgByte = wire->read();

    if (cfgByte & 0x80) return NAN;  // Conversion not ready

    int16_t rawValue = (highByte << 8) | lowByte;
    return rawValue * (2.048 / 32768.0);
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_HB, OUTPUT);

    // Initialize I2C buses
    WireSensorA.begin();
    WireSensorB.begin();

    // Configure pin peripherals
    portSensorsA.setPinPeripheralStates();
    portSensorsB.setPinPeripheralAltStates();
}

void loop() {
    digitalWrite(LED_HB, !digitalRead(LED_HB));

    // Read all 4 channels
    float v1 = readMCP3426Channel(&WireSensorA, 0);
    float v2 = readMCP3426Channel(&WireSensorA, 1);
    float v3 = readMCP3426Channel(&WireSensorB, 0);
    float v4 = readMCP3426Channel(&WireSensorB, 1);

    Serial.println("Temperature Sensor Voltages:");
    Serial.print("  Bus A CH1: "); Serial.println(v1, 4);
    Serial.print("  Bus A CH2: "); Serial.println(v2, 4);
    Serial.print("  Bus B CH1: "); Serial.println(v3, 4);
    Serial.print("  Bus B CH2: "); Serial.println(v4, 4);

    delay(1000);
}
```

### Converting Voltage to Temperature

The voltage-to-temperature conversion depends on the specific temperature sensor used. For common NTC thermistors with voltage divider:

```cpp
float voltageToTemperature(float voltage, float vRef, float rRef, float beta, float t0) {
    // Calculate thermistor resistance from voltage divider
    float resistance = rRef * voltage / (vRef - voltage);

    // Steinhart-Hart equation (simplified B-parameter)
    float tempK = 1.0 / (1.0/t0 + (1.0/beta) * log(resistance/rRef));
    float tempC = tempK - 273.15;

    return tempC;
}

// Example usage with 10k NTC thermistor:
// float tempC = voltageToTemperature(voltage, 3.3, 10000.0, 3950.0, 298.15);
```

---

## I2C Device Scanning

Use the WireScanner utility to verify connected devices:

```cpp
WireScanner scannerA(&WireSensorA, "Sensors A");
WireScanner scannerB(&WireSensorB, "Sensors B");

void setup() {
    // ... initialization code ...

    Serial.println("Scanning I2C buses...");
    scannerA.scan();
    scannerB.scan();
}
```

**Expected Output:**
```
Scanning I2C buses...
[Sensors A] Scanning...
  Device found at 0x68
[Sensors B] Scanning...
  Device found at 0x68
```

---

## Channel Summary

| Channel ID | I2C Bus | MCP3426 Channel | Description |
|------------|---------|-----------------|-------------|
| 0 | Bus A | CH1 | Temperature Sensor 1 |
| 1 | Bus A | CH2 | Temperature Sensor 2 |
| 2 | Bus B | CH1 | Temperature Sensor 3 |
| 3 | Bus B | CH2 | Temperature Sensor 4 |

---

## Dependencies

- Wire.h (I2C communication)
- WireScanner.h (I2C device scanning)
- TwiPinHelper.h (Pin peripheral configuration)
- wiring_private.h (SAM microcontroller pin definitions)

---

## Hardware Notes

### MCP3426 Address Configuration

The MCP3426 I2C address is set by the A0 pin:
| A0 Pin | Address |
|--------|---------|
| GND | 0x68 |
| VDD | 0x69 |
| SDA | 0x6A |
| SCL | 0x6B |

### SERCOM Configuration (SAM D21)

| I2C Bus | SERCOM | SDA Function | SCL Function |
|---------|--------|--------------|--------------|
| Bus A | SERCOM1 | PIO_SERCOM | PIO_SERCOM |
| Bus B | SERCOM4 | PIO_SERCOM_ALT | PIO_SERCOM_ALT |
