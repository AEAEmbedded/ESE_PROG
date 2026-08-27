/*
    Arduino Terminal application that adheres to the SOLID-principles.
  
    Johan Korten johan.korten@han.nl
    Oct 2023 v1.0

    DesignPattern (Singleton) added

*/

#include "SerialConnectionManager.h"
#include "IOHandler.h"
#include "UserInterface.h"

SerialConnectionManager* connManager = SerialConnectionManager::getInstance();
IOHandler ioHandler(*connManager);
UserInterface userInterface(ioHandler);

unsigned long lastUpdateTime = 0;
const unsigned long updateInterval = 1000; // 1 second

void myCallback() {
    unsigned long currentMillis = millis();

    // Check if it's time to update the message
    if(currentMillis - lastUpdateTime > updateInterval) {
        userInterface.displayMessage("Hello! Type anything and press Enter:");
        String input = userInterface.getUserInput();
        if (input.length() > 0) {
            userInterface.displayMessage("You typed: " + input);
        }

        lastUpdateTime = currentMillis;
    }
}


void setup() {
    connManager->connect();
    userInterface.setCallback(myCallback); // Set the callback
    userInterface.start(); // This will call the callback
}

void loop() {
    // Nothing here for this demo, as the callback is running indefinitely
}