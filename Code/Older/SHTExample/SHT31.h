#ifndef SHT31_H
#define SHT31_H

#include <cstdint>

class SHT31 {
public:
    SHT31(const char* i2cDevice = "/dev/i2c-1", uint8_t i2cAddr = 0x44);
    ~SHT31();

    bool begin();
    bool readSensor(float &temperature, float &humidity);

private:
    const char* _i2cDevice;
    uint8_t _i2cAddr;
    int _fd;
};

#endif // SHT31_H