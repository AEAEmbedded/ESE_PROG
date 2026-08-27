#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "IOHandler.h"

class UserInterface {
private:
    IOHandler& ioHandler;
public:
    UserInterface(IOHandler& handler) : ioHandler(handler) {}

    void start() {
        while (true) { // Main loop
            displayMessage("Hello! Type anything and press Enter:");
            String input = getUserInput();
            displayMessage("You typed: " + input);
        }
    }

    void displayMessage(const String& message) {
        ioHandler.write(message);
    }

    String getUserInput() {
        return ioHandler.read();
    }
};

#endif
