#ifndef SERIAL_CONNECTION_MANAGER_H
#define SERIAL_CONNECTION_MANAGER_H

#include "IConnectionManager.h"

class SerialConnectionManager : public IConnectionManager {

private:
    static SerialConnectionManager* instance;

    SerialConnectionManager() {
        // private constructor
    }

public:

    static SerialConnectionManager* getInstance() {
        if (!instance) {
            instance = new SerialConnectionManager();
        }
        return instance;
    }

    void connect() override {
        Serial.begin(115200);
    }
    void disconnect() override {
        // Arduino's Serial doesn't have a straightforward disconnect
        // You might just leave this empty or end serial & delay for a bit
    }
    bool isConnected() const override {
        // For demonstration. Actual check might be more complex.
        return Serial;
    }
};

SerialConnectionManager* SerialConnectionManager::instance = nullptr;

#endif