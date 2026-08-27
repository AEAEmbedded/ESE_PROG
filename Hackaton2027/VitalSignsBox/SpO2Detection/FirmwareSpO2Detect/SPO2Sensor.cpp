/*
    SPO2Sensor.cpp

    SpO2 Sensor Connection Detection Implementation

    Johan Korten
    for HAN ESE / WKZ Hackaton Challenge 2026
*/

#include "SPO2Sensor.h"

SPO2Sensor::SPO2Sensor(uint8_t pin, uint8_t ledPin, uint16_t threshold)
    : _pin(pin)
    , _ledPin(ledPin)
    , _threshold(threshold)
    , _rawValue(0)
    , _connected(false)
    , _ledState(false)
{
}

void SPO2Sensor::begin() {
    pinMode(_pin, INPUT);
    pinMode(_ledPin, OUTPUT);
    digitalWrite(_ledPin, LOW);
}

void SPO2Sensor::update() {
    _rawValue = analogRead(_pin);
    // Sensor connected when line is pulled low (below threshold)
    _connected = (_rawValue < _threshold);
}

bool SPO2Sensor::isConnected() const {
    return _connected;
}

uint16_t SPO2Sensor::getRawValue() const {
    return _rawValue;
}

uint8_t SPO2Sensor::getHighByte() const {
    return (_rawValue >> 8) & 0xFF;
}

uint8_t SPO2Sensor::getLowByte() const {
    return _rawValue & 0xFF;
}

uint8_t SPO2Sensor::getStatusByte() const {
    return _connected ? 1 : 0;
}

void SPO2Sensor::setThreshold(uint16_t threshold) {
    _threshold = threshold;
}

uint16_t SPO2Sensor::getThreshold() const {
    return _threshold;
}

void SPO2Sensor::ledOn() {
    _ledState = true;
    digitalWrite(_ledPin, HIGH);
}

void SPO2Sensor::ledOff() {
    _ledState = false;
    digitalWrite(_ledPin, LOW);
}

void SPO2Sensor::setLed(bool state) {
    _ledState = state;
    digitalWrite(_ledPin, state ? HIGH : LOW);
}

bool SPO2Sensor::isLedOn() const {
    return _ledState;
}
