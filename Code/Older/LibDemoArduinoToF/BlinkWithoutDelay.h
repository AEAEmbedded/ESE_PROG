#ifndef BlinkWithoutDelay_h
#define BlinkWithoutDelay_h

#include "Arduino.h"

class BlinkWithoutDelay {
public:
    BlinkWithoutDelay(int pin, unsigned long interval);

    void on();
    void off();
    void update();

private:
    int _pin;
    unsigned long _interval;
    unsigned long _previousMillis;
    bool _ledState;
};

#endif