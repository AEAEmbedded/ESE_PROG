/*
    Johan Korten
    for HAN ESE, WKZ, Capgemini / GET Hackaton Challenge 2025

    V1.0 Jan 2025

*/


#include "ECGSensor.h"

ECGSensor::ECGSensor(uint8_t pin) : pin(pin), value(0) {}

void ECGSensor::read() {
  value = analogRead(pin);
}

uint16_t ECGSensor::getValue() const {
  return value;
}

uint8_t ECGSensor::getHighByte() const {
  return (value >> 8) & 0xFF;
}

uint8_t ECGSensor::getLowByte() const {
  return value & 0xFF;
}