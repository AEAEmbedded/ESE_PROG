#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "IOHandler.h"

typedef void (*CallbackType)();

class UserInterface {
private:
  IOHandler& ioHandler;
  CallbackType callback;

public:
  UserInterface(IOHandler& handler)
    : ioHandler(handler), callback(nullptr) {}

  void setCallback(CallbackType cb) {
    callback = cb;
  }

  void start() {
    if (callback) callback();  // Only call if callback is set
  }

  void displayMessage(const String& message) {
    ioHandler.write(message);
  }

  String getUserInput() {
    return ioHandler.read();
  }
};

#endif
