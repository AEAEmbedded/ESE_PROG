#include "SHT31.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>


SHT31::SHT31(const char* i2cDevice, uint8_t i2cAddr)
    : _i2cDevice(i2cDevice), _i2cAddr(i2cAddr), _fd(-1) {}

SHT31::~SHT31() {
    if (_fd != -1) close(_fd);
}

bool SHT31::begin() {
    _fd = open(_i2cDevice, O_RDWR);
    if (_fd == -1) return false;
    if (ioctl(_fd, I2C_SLAVE, _i2cAddr) < 0) return false;
    return true;
}

bool SHT31::readSensor(float &temperature, float &humidity) {
    uint8_t cmd[] = {0x24, 0x00}; // High repeatability measurement command
    if (write(_fd, cmd, 2) != 2) return false;
    usleep(15000);  // 15ms measurement delay

    uint8_t data[6];
    if (read(_fd, data, 6) != 6) return false;

    int rawTemp = (data[0] << 8) | data[1];
    int rawHum = (data[3] << 8) | data[4];

    // Convert raw values to actual temperature and humidity
    temperature = 175.0f * rawTemp / 65535.0f - 45.0f;
    humidity = 100.0f * rawHum / 65535.0f;

    return true;
}