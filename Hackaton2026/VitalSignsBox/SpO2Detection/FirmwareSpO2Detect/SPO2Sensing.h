/*
    SPO2Sensing.h

    SpO2 Sensor Sensing Helper Functions

    Johan Korten
    for HAN ESE / WKZ Hackaton Challenge 2026

    V1.0 Jan 2026
*/

#ifndef SPO2_SENSING_H
#define SPO2_SENSING_H

#include "SPO2Sensor.h"

// Pin definitions
#define SENSOR_SPO2 A2  // SPO2_CONNECTION_A2

// External reference to sensor instance (defined in main .ino)
extern SPO2Sensor spo2Sensor;

/**
 * Read and report SpO2 sensor connection status via Serial
 */
void readAndReportSpO2Status() {
    Serial.print("SpO2 Sensor: ");
    Serial.print(spo2Sensor.isConnected() ? "CONNECTED" : "DISCONNECTED");
    Serial.print(" (raw: ");
    Serial.print(spo2Sensor.getRawValue());
    Serial.print(", LED: ");
    Serial.print(spo2Sensor.isLedOn() ? "ON" : "OFF");
    Serial.println(")");
}

#endif // SPO2_SENSING_H
