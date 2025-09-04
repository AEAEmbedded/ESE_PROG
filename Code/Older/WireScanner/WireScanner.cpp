#include <utility>
#include "WireScanner.h"

const std::pair<int, const char*> deviceNames[] = {
    {0x25, "Sensirion SDP8xx-500Pa"},
    {0x26, "Sensirion SDP8xx-501Pa"},
    {0x29, "DLC-L01G-U2 or VL6180"},
    {0x40, "Sensirion SDP610-500Pa"},
    {0x50, "FRAM/EEPROM"},
    {0x51, "More memory? Could be 1M FRAM"},
    {0x7C, "RESERVED"}
};

WireScanner::WireScanner(TwoWire *wire, const char* label) : _wire(wire), _label(label) {}


void WireScanner::scan() {
    Serial.print("Scanning bus ");
    Serial.println(_label);

    for (byte address = 1; address < 127; address++) {
        if (reportDevicesWithAddress(address)) {
            Serial.print("I2C device found at address 0x");
            if (address < 16)
                Serial.print("0");
            Serial.print(address, HEX);
            Serial.println("  !");
            printDeviceName(address);
        }
    }

    Serial.println("done\n");
}

bool WireScanner::reportDevicesWithAddress(byte deviceAddress) {
    _wire->beginTransmission(deviceAddress);
    byte error = _wire->endTransmission();
    return (error == 0);
}

void WireScanner::printDeviceName(int address) {
    for (const auto& pair : deviceNames) {
        if (pair.first == address) {
            Serial.println(pair.second);
            return; // Found, exit early
        }
    }
    // Handle unrecognized addresses if needed
    Serial.print("Unknown Device found");
    Serial.println(address, HEX);
}

// Default implementation of scanForDevice
bool WireScanner::scanForDevice(byte address) {
    return reportDevicesWithAddress(address);
}
