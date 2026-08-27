/*
    Arduino Terminal application that adheres to the SOLID-principles.
  
    Johan Korten johan.korten@han.nl
    Oct 2023 v1.0

*/

#include "SerialConnectionManager.h"
#include "IOHandler.h"
#include "UserInterface.h"

SerialConnectionManager connManager;
IOHandler ioHandler(connManager);
UserInterface userInterface(ioHandler);

void setup() {
  connManager.connect();
  userInterface.start();  // This will run indefinitely for this demo
}

void loop() {
  // Nothing here for this demo, as userInterface.start() is running indefinitely
}
