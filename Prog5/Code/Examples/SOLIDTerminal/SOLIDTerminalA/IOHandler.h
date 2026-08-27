#ifndef IOHANDLER_H
#define IOHANDLER_H

#include "IConnectionManager.h"

class IOHandler {
private:
    IConnectionManager& connManager;
public:
    IOHandler(IConnectionManager& manager) : connManager(manager) {}

    void write(const String& data) {
        if (connManager.isConnected()) {
            Serial.println(data);
        }
    }

    String read() {
        if (connManager.isConnected() && Serial.available()) {
            return Serial.readStringUntil('\n');
        }
        return "";
    }
};

#endif
