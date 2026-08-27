/*
    Johan Korten
    for HAN ESE, WKZ, Capgemini / GET Hackaton Challenge 2025

    V1.0 Jan 2025

    Based on Wire Slave Sender and Receiver by Nicholas Zambetti <http://www.zambetti.com> March 2006

*/ 

#include <Wire.h>
#include "HeartBeat.h"
#include "ECGSensing.h"

#include "SerialHelper.h"
#include "ECGSensor.h"

#define ECG_MODULE_ADDR 0x2A
#define HEARTBEAT_LEDPIN 14
#define DEFAULT_HEARTBEAT_INTERVAL 1000

// Define sensors
ECGSensor sensorLL(A1); // PB08 red
ECGSensor sensorLA(A2); // PB09 black
ECGSensor sensorRA(A3); // PA04 white

HeartBeat heartBeat = HeartBeat(); // (HEARTBEAT_LEDPIN, DEFAULT_HEARTBEAT_INTERVAL) ;

void setup() {
  heartBeat.begin();
  initSerial();
  Wire.begin(ECG_MODULE_ADDR);     // join i2c bus with address #4
  Wire.onReceive(receiveEvent);  // register event maybe for future usage
  Wire.onRequest(requestEvent);    // register event
}

void loop() {
  heartBeat.blink();
  readAndReportECGLeads();
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
// Maybe useful for future usage

void receiveEvent(int howMany) {
  (void)howMany;  // avoid compiler warning about unused parameter

  while (1 < Wire.available())  // loop through all but the last
  {
    char c = Wire.read();  // receive byte as a character
    Serial.print(c);       // print the character
  }
  int x = Wire.read();  // receive byte as an integer
  Serial.println(x);    // print the integer
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent()
{

  Wire.write("012345"); // respond with message of 6 bytes (HLL, LLL, HLA, LLA, HRA, LRA)
                        // as expected by master
}