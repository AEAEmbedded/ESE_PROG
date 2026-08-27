#ifndef ICONNECTION_MANAGER_H
#define ICONNECTION_MANAGER_H

class IConnectionManager {
public:
    virtual void connect() = 0;
    virtual void disconnect() = 0;
    virtual bool isConnected() const = 0;
    virtual ~IConnectionManager() {}
};

#endif