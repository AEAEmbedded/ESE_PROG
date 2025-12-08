/*
    SerialHelper.h

    Serial communication initialization helper

    Johan Korten
    for HAN ESE / WKZ Hackaton Challenge 2026

    V1.0 Jan 2026
*/

#ifndef SerialHelper_h
#define SerialHelper_h

#define SERIAL_BAUDRATE 115200

void initSerial() {
  Serial.begin(SERIAL_BAUDRATE);
}

#endif
