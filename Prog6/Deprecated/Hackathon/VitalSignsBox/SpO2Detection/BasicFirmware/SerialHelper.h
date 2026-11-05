/*
    Johan Korten
    for HAN ESE, WKZ, Capgemini / GET Hackaton Challenge 2025

    V1.0 Jan 2025

*/

#define SERIAL_BAUDRATE 115200

void initSerial() {
  Serial.begin(SERIAL_BAUDRATE);
}