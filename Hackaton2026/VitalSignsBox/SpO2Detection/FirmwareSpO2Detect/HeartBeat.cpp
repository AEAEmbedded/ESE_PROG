/*
    HeartBeat.cpp

    Non-blocking LED heartbeat indicator implementation

    Johan Korten
    for HAN ESE / WKZ Hackaton Challenge 2026

    V1.0 Jan 2026
*/

#include "HeartBeat.h"
#include "Arduino.h"

HeartBeat::HeartBeat() {
  _ledPin = 14;
  _interval = 100;
}

HeartBeat::HeartBeat(unsigned int ledPin, long interval) {
   _ledPin = ledPin;
   _interval = interval;
}

void HeartBeat::begin() {
    pinMode(_ledPin, OUTPUT);
}

void HeartBeat::blink() {
  unsigned long currentMillis = millis();
  if (currentMillis - _previousMillis >= _interval) {
    _previousMillis = currentMillis;
    if (digitalRead(_ledPin) == LOW) {
      digitalWrite(_ledPin, HIGH);
    } else {
      digitalWrite(_ledPin, LOW);
    }
  }
}
