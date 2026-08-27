/*
    Johan Korten
    for HAN ESE, WKZ, Capgemini / GET Hackaton Challenge 2025

    V1.0 Jan 2025

*/ 

// Sensor Definitions:

#define SENSOR_LL A1  // PB08 red
#define SENSOR_LA A2  // PB09 black
#define SENSOR_RA A3  // PA04 white


void readAndReportECGLeads() {
  Serial.print(analogRead(SENSOR_LL));
  Serial.print(" ");
  Serial.print(analogRead(SENSOR_LA));
  Serial.print(" ");
  Serial.print(analogRead(SENSOR_RA));
  Serial.println();
}
