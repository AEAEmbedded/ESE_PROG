# VitalSignsBox

This repo provides basic implementations for the VitalSignsBox modules.

## Modules

### ECGLeadDetection

Detects ECG lead connections (Left-Leg, Left-Arm, Right-Arm) via analog sensors.

- **BasicFirmware** - Basic implementation for getting started with the board
- **FirmwareECGLeadDetect** - Production firmware that reads three sensors (10-bit resolution), provides data as 6 bytes via I2C (address: 0x2A)

See [ECGLeadDetection/API.md](ECGLeadDetection/API.md) for full API documentation.

### SpO2Detection

Detects SpO2 sensor connection via pull-up resistor detection (does not measure actual SpO2).

- **FirmwareSpO2Detect** - Detects sensor presence on SPO2_CONNECTION_A2 (10k pull-up to 3.3V), controls RED LED on SPO2_LED_D12, provides status via I2C (address: 0x2B)

See [SpO2Detection/API.md](SpO2Detection/API.md) for full API documentation.

### TemperatureSensorsDetection

Reads temperature sensor data via MCP3426 16-bit ADC chips.

- **BasicImplementation_TempSensors** - Reads CH1+ and CH2+ channels from MCP3426 sensors on dual I2C buses (same address 0x68, separate buses A and B)

See [TemperatureSensorsDetection/API.md](TemperatureSensorsDetection/API.md) for full API documentation.

### Utils

Utility libraries shared across modules.

- **WireScannerLibrary** - I2C bus scanning and pin configuration utilities

See [Utils/WireScannerLibrary/API.md](Utils/WireScannerLibrary/API.md) for full API documentation.

## I2C Address Summary

| Module | Address | Data Size |
|--------|---------|-----------|
| ECGLeadDetection | 0x2A | 6 bytes |
| SpO2Detection | 0x2B | 4 bytes |
| MCP3426 (Temp) | 0x68 | 3 bytes |

## Hardware

All boards are based on the [Adafruit Feather M0 Express](https://www.adafruit.com/product/3403).

Getting started guide: https://learn.adafruit.com/adafruit-feather-m0-express-designed-for-circuit-python-circuitpython
