/*
    Arduino Terminal application that adheres to the SOLID-principles.
  
    Johan Korten johan.korten@han.nl
    Oct 2023 v1.0

    Added a call-back method

*/

#include "SerialConnectionManager.h"
#include "IOHandler.h"
#include "UserInterface.h"

SerialConnectionManager connManager;
IOHandler ioHandler(connManager);
UserInterface userInterface(ioHandler);

void myCallback() {
    while (true) { // Main loop
        userInterface.displayMessage("Hello! Type anything and press Enter:");
        String input = userInterface.getUserInput();
        userInterface.displayMessage("You typed: " + input);
    }
}

void setup() {
    connManager.connect();
    userInterface.setCallback(myCallback); // Set the callback
    userInterface.start(); // This will call the callback
}

void loop() {
    // Nothing here for this demo, as the callback is running indefinitely
}