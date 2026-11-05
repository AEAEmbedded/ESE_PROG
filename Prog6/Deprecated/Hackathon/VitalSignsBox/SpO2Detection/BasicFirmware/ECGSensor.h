/*
    Johan Korten
    for HAN ESE, WKZ, Capgemini / GET Hackaton Challenge 2025

    V1.0 Jan 2025

*/

#ifndef ECG_SENSOR_H
#define ECG_SENSOR_H

#include "Arduino.h"

class ECGSensor {
public:
  ECGSensor(uint8_t pin);
  void read();
  uint16_t getValue() const;
  uint8_t getHighByte() const;
  uint8_t getLowByte() const;

private:
  uint8_t pin;
  uint16_t value;
};

#endif // ECG_SENSOR_H
