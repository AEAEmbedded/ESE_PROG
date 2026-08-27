#include "BlinkWithoutDelay.h"

BlinkWithoutDelay::BlinkWithoutDelay(int pin, unsigned long interval) :
  _pin(pin),
  _interval(interval),
  _previousMillis(0),
  _ledState(LOW)
{
  pinMode(_pin, OUTPUT);
}

void BlinkWithoutDelay::on() {
  _ledState = HIGH;
  digitalWrite(_pin, _ledState);
}

void BlinkWithoutDelay::off() {
  _ledState = LOW;
  digitalWrite(_pin, _ledState);
}

void BlinkWithoutDelay::update() {
  unsigned long currentMillis = millis();

  if (currentMillis - _previousMillis >= _interval) {
    _previousMillis = currentMillis;

    // Toggle the LED state
    _ledState = !_ledState;
    digitalWrite(_pin, _ledState);
  }
}