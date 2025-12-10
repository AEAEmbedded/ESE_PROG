# Wire Scanner Library - API Documentation

## Overview

The Wire Scanner Library provides utilities for I2C bus scanning and pin configuration on SAM D21 microcontrollers. It includes two classes:

- **WireScanner** - Scans I2C buses and identifies connected devices
- **TwiPinPair** - Configures pins for I2C (TWI) operation

## Module Location

```
Utils/
└── WireScannerLibrary/
    └── Library/
        ├── WireScanner.h
        ├── WireScanner.cpp
        ├── TwiPinHelper.h
        └── TwiPinHelper.cpp
```

---

## WireScanner Class

Scans an I2C bus and reports all connected devices with optional device identification.

**Header:** `WireScanner.h`

### Constructor

```cpp
WireScanner(TwoWire *wire, const char* label);
```

Creates a scanner instance for the specified I2C bus.

**Parameters:**
- `wire` - Pointer to the TwoWire I2C bus instance
- `label` - Human-readable name for the bus (used in output)

**Example:**
```cpp
WireScanner scanner(&Wire, "Main Bus");
WireScanner scannerA(&WireSensorA, "Sensors A");
```

### Methods

#### scan()

```cpp
void scan();
```

Scans all valid I2C addresses (0x01-0x7F) and prints found devices to Serial.

**Output Format:**
```
[Label] Scanning...
  Device found at 0xNN: DeviceName
  Device found at 0xNN
[Label] Scan complete. N device(s) found.
```

**Example:**
```cpp
void setup() {
    Serial.begin(115200);
    Wire.begin();

    WireScanner scanner(&Wire, "Main");
    scanner.scan();
}
```

**Sample Output:**
```
[Main] Scanning...
  Device found at 0x68: MCP3426
  Device found at 0x50: FRAM/EEPROM
[Main] Scan complete. 2 device(s) found.
```

#### printDeviceName()

```cpp
void printDeviceName(int address);
```

Prints the known device name for a given I2C address.

**Parameters:**
- `address` - I2C device address (0x00-0x7F)

**Known Devices:**

| Address | Device Name |
|---------|-------------|
| 0x25 | Sensirion SDP8xx-500Pa |
| 0x26 | Sensirion SDP8xx-501Pa |
| 0x29 | DLC-L01G-U2 or VL6180 |
| 0x40 | Sensirion SDP610-500Pa |
| 0x50 | FRAM/EEPROM |
| 0x51 | 1M FRAM |
| 0x7C | RESERVED |

**Example:**
```cpp
scanner.printDeviceName(0x50);  // Prints ": FRAM/EEPROM"
```

---

## TwiPinPair Class

Configures GPIO pins for I2C (TWI) peripheral operation on SAM D21 microcontrollers.

**Header:** `TwiPinHelper.h`

### Constructor

```cpp
TwiPinPair(uint32_t dataPin, uint32_t clockPin);
```

Creates a pin pair configuration for I2C operation.

**Parameters:**
- `dataPin` - GPIO pin number for SDA (data line)
- `clockPin` - GPIO pin number for SCL (clock line)

**Example:**
```cpp
TwiPinPair pins(28, 39);  // SDA=28 (PA12), SCL=39 (PA13)
```

### Methods

#### getPortPinSDA()

```cpp
String getPortPinSDA();
```

Returns the port designation for the SDA pin.

**Returns:** `String` - Port and pin name (e.g., "PA12", "PB08")

**Example:**
```cpp
TwiPinPair pins(28, 39);
Serial.println(pins.getPortPinSDA());  // Prints "PA12"
```

#### getPortPinSCL()

```cpp
String getPortPinSCL();
```

Returns the port designation for the SCL pin.

**Returns:** `String` - Port and pin name (e.g., "PA13", "PB09")

**Example:**
```cpp
TwiPinPair pins(28, 39);
Serial.println(pins.getPortPinSCL());  // Prints "PA13"
```

#### setPinPeripheralStates()

```cpp
void setPinPeripheralStates();
```

Configures both pins as `PIO_SERCOM` peripheral function. Use this for primary SERCOM pin assignments.

**Example:**
```cpp
TwiPinPair pins(28, 39);
pins.setPinPeripheralStates();  // Configure as PIO_SERCOM
```

#### setPinPeripheralAltStates()

```cpp
void setPinPeripheralAltStates();
```

Configures both pins as `PIO_SERCOM_ALT` peripheral function. Use this for alternate SERCOM pin assignments.

**Example:**
```cpp
TwiPinPair pins(11, 13);
pins.setPinPeripheralAltStates();  // Configure as PIO_SERCOM_ALT
```

---

## Complete Usage Example

### Multi-Bus I2C Scanning

```cpp
#include <Wire.h>
#include "WireScanner.h"
#include "TwiPinHelper.h"

// Pin definitions for dual I2C buses
#define W1_SDA 28  // PA12
#define W1_SCL 39  // PA13
#define W2_SDA 11  // PA16
#define W2_SCL 13  // PA17

// Create I2C bus instances
TwoWire WireSensorA(&sercom1, W1_SDA, W1_SCL);
TwoWire WireSensorB(&sercom4, W2_SDA, W2_SCL);

// Pin configuration helpers
TwiPinPair pinsA(W1_SDA, W1_SCL);
TwiPinPair pinsB(W2_SDA, W2_SCL);

// Scanners
WireScanner scannerA(&WireSensorA, "Bus A");
WireScanner scannerB(&WireSensorB, "Bus B");

void setup() {
    Serial.begin(115200);
    while (!Serial) delay(10);

    // Print pin assignments
    Serial.println("I2C Pin Configuration:");
    Serial.print("  Bus A: SDA=");
    Serial.print(pinsA.getPortPinSDA());
    Serial.print(", SCL=");
    Serial.println(pinsA.getPortPinSCL());

    Serial.print("  Bus B: SDA=");
    Serial.print(pinsB.getPortPinSDA());
    Serial.print(", SCL=");
    Serial.println(pinsB.getPortPinSCL());

    // Initialize I2C buses
    WireSensorA.begin();
    WireSensorB.begin();

    // Configure pin peripherals
    pinsA.setPinPeripheralStates();
    pinsB.setPinPeripheralAltStates();

    // Scan for devices
    Serial.println("\nScanning I2C buses...\n");
    scannerA.scan();
    Serial.println();
    scannerB.scan();
}

void loop() {
    // Your application code
}
```

**Sample Output:**
```
I2C Pin Configuration:
  Bus A: SDA=PA12, SCL=PA13
  Bus B: SDA=PA16, SCL=PA17

Scanning I2C buses...

[Bus A] Scanning...
  Device found at 0x68
[Bus A] Scan complete. 1 device(s) found.

[Bus B] Scanning...
  Device found at 0x68
[Bus B] Scan complete. 1 device(s) found.
```

---

## SAM D21 SERCOM Reference

### Pin Peripheral Functions

| Function | Use Case |
|----------|----------|
| `PIO_SERCOM` | Primary SERCOM pin assignment |
| `PIO_SERCOM_ALT` | Alternate SERCOM pin assignment |

### Common SERCOM Assignments

| SERCOM | Primary Pins | Alternate Pins |
|--------|--------------|----------------|
| SERCOM1 | PA00, PA01 | PA16, PA17 |
| SERCOM4 | PB12, PB13 | PA12, PA13 |

> **Note:** Consult the SAM D21 datasheet for complete SERCOM pin multiplexing options.

---

## Adding New Device Recognition

To add recognition for additional I2C devices, modify `WireScanner.cpp`:

```cpp
void WireScanner::printDeviceName(int address) {
    switch (address) {
        case 0x25: Serial.print(": Sensirion SDP8xx-500Pa"); break;
        case 0x26: Serial.print(": Sensirion SDP8xx-501Pa"); break;
        case 0x29: Serial.print(": DLC-L01G-U2 or VL6180"); break;
        case 0x40: Serial.print(": Sensirion SDP610-500Pa"); break;
        case 0x50: Serial.print(": FRAM/EEPROM"); break;
        case 0x51: Serial.print(": 1M FRAM"); break;
        case 0x68: Serial.print(": MCP3426 ADC"); break;  // Add new device
        case 0x7C: Serial.print(": RESERVED"); break;
        default: break;  // Unknown device
    }
}
```

---

## Dependencies

- Arduino.h (standard Arduino library)
- Wire.h (I2C communication)
- wiring_private.h (SAM D21 pin peripheral configuration)
