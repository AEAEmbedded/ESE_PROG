#ifndef WireScanner_h
#define WireScanner_h

#include <Arduino.h>
#include <Wire.h>
#include "TwiPinHelper.h"

class WireScanner {
public:
    WireScanner(TwoWire *wire, const char* label);

    void scan();
    void printDeviceName(int address);

    // New virtual function to allow overriding the scanning behavior
    virtual bool scanForDevice(byte address);

private:
    TwoWire *_wire;
    const char* _label;
    bool reportDevicesWithAddress(byte deviceAddress);
};

#endif
