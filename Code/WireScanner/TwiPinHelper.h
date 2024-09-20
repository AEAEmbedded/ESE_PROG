#ifndef TwiPinHelper_h
#define TwiPinHelper_h

#include <Arduino.h>
#include "wiring_private.h" 

class TwiPinPair {
public:
    TwiPinPair(uint32_t dataPin, uint32_t clockPin);

    String getPortPinSDA();
    String getPortPinSCL();

    void setPinPeripheralStates();
    void setPinPeripheralAltStates();

private:
    uint32_t _dataPin;
    uint32_t _clockPin;
    String getPortPin(uint32_t ulPin); 
};

#endif