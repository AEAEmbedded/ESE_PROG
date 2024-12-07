# Patient Monitoring System on Raspberry Pi 3

This project involves creating a patient monitoring system using Raspberry Pi 3, which interfaces with various medical sensor modules (ECG, SpO2, NIBP, Temperature) via I2C and other communication protocols. The system features a simple and intuitive user interface for real-time monitoring.

### Table of Contents

- Overview
- Hardware Requirements
- Software Requirements
- Setup Guide
- System Architecture
- Usage
- Future Enhancements
- Overview

## The Patient Monitoring System collects and displays vital signs such as:

- ECG (Electrocardiogram)
- SpO2 (Oxygen Saturation)
- NIBP (Non-Invasive Blood Pressure)
- Temperature

The system is modular and allows integration of additional sensors through standardized adapters and communication protocols.

## Hardware Requirements

Raspberry Pi 3 (or higher) with GPIO-enabled interface

### Sensor Modules:
- ECG: ecg_leaddetection_module
- SpO2: spo2_module
- Temperature: temp_module
- NIBP: Pressure_Module

### Adapters:
- SensorAdapter Quad PTSM4
- SimModule Adapter

### Connections:
- I2C: SDA (GPIO2) and SCL (GPIO3)
- UART or SPI (if required by modules)

Refer to the provided .brd and .sch files for hardware schematics.

  * [hardware](./)
    * [Adapters](./Adapters) - PCB schematics and board files for adapters
    * [SimModule_ECG](./SimModule_ECG) - ECG sensor module files
    * [SimModule_NIBP](./SimModule_NIBP) - Blood pressure (NIBP) module files
    * [SimModule_SpO2](./SimModule_SpO2) - SpO2 (Oxygen saturation) module files
    * [SimModule_Temperature](./SimModule_Temperature) - Temperature module files
  * [README.md](README.md) - Documentation file (this file)