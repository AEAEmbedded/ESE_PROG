/*
    Johan Korten
    for HAN ESE, WKZ, Capgemini / GET Hackaton Challenge 2025

    V1.0 Jan 2025

    Based on Wire Slave Sender and Receiver by Nicholas Zambetti <http://www.zambetti.com> March 2006

    For your convenience we implemented:
    - continous reading of the Left-Leg (LL, RED), Left-Arm (LA, BLACK) and Right-Arm (WHITE) sensors
    - we provide six bytes to the client, for each sensor 

*/

#include <Wire.h>
#include "HeartBeat.h"
#include "ECGSensing.h"

#include "SerialHelper.h"
#include "ECGSensor.h"

#define ECG_MODULE_ADDR 0x2A
#define HEARTBEAT_LEDPIN 14
#define DEFAULT_HEARTBEAT_INTERVAL 1000

#define TESTING 1 // This enables/disables serial output.
                  // Please make TESTING == 0 when NOT in testing mode, otherise it needs to be 1. 

// Define sensors
ECGSensor sensorLL(A1);  // PB08 red
ECGSensor sensorLA(A2);  // PB09 black
ECGSensor sensorRA(A3);  // PA04 white

#define NUM_SENSOR_BYTES 6

HeartBeat heartBeat = HeartBeat();  // (HEARTBEAT_LEDPIN, DEFAULT_HEARTBEAT_INTERVAL) ;

void setup() {
  heartBeat.begin();
  initSerial();
  Wire.begin(ECG_MODULE_ADDR);   // join i2c bus with address #4
  Wire.onReceive(receiveEvent);  // register event maybe for future usage
  Wire.onRequest(requestEvent);  // register event
}

void loop() {
  heartBeat.blink();

  // Update all sensors
  sensorLL.read();
  sensorLA.read();
  sensorRA.read();

  if (TESTING) readAndReportECGLeads();  // Just for testing / development
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
// Maybe useful for future usage

void receiveEvent(int howMany) {
  (void)howMany;  // avoid compiler warning about unused parameter

  while (1 < Wire.available())  // loop through all but the last
  {
    char c = Wire.read();  // receive byte as a character
    if (TESTING) Serial.print(c);       // print the character
  }
  int x = Wire.read();  // receive byte as an integer
  if (TESTING) Serial.println(x);    // print the integer
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  uint8_t response[NUM_SENSOR_BYTES];

  // Helper function to add sensor data to the buffer
  auto populateBuffer = [](uint8_t* buffer, const ECGSensor& sensor, int offset) {
    buffer[offset] = sensor.getHighByte();
    buffer[offset + 1] = sensor.getLowByte();
  };

  // Populate response buffer
  populateBuffer(response, sensorLL, 0);
  populateBuffer(response, sensorLA, 2);
  populateBuffer(response, sensorRA, 4);

  // Send the response over I2C
  Wire.write(response, NUM_SENSOR_BYTES);
}