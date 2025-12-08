/*
    Johan Korten
    for HAN ESE, WKZ, Capgemini / GET Hackaton Challenge 2025

    V1.0 Jan 2025

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
      digitalWrite(_ledPin, HIGH);   // Turn it on
    } else {
      digitalWrite(_ledPin, LOW);    // Turn it off
    }
  }
}