#include "TwiPinHelper.h"

TwiPinPair::TwiPinPair(uint32_t dataPin, uint32_t clockPin) : _dataPin(dataPin), _clockPin(clockPin) {}

String TwiPinPair::getPortPin(uint32_t ulPin) {
    if (g_APinDescription[ulPin].ulPinType == PIO_NOT_A_PIN) {
        return "NA";
    }

    EPortType port = g_APinDescription[ulPin].ulPort;
    uint32_t pin = g_APinDescription[ulPin].ulPin;
    String result = "P";
    char portLetter = 'A';
    portLetter += port;
    result += portLetter;
    result += String(pin);
    return result;
}

String TwiPinPair::getPortPinSDA() {
    return getPortPin(_dataPin);
}

String TwiPinPair::getPortPinSCL() {
    return getPortPin(_clockPin);
}

void TwiPinPair::setPinPeripheralStates() {
    pinPeripheral(_dataPin, PIO_SERCOM);
    pinPeripheral(_clockPin, PIO_SERCOM);
}

void TwiPinPair::setPinPeripheralAltStates() {
    pinPeripheral(_dataPin, PIO_SERCOM_ALT);
    pinPeripheral(_clockPin, PIO_SERCOM_ALT);
}