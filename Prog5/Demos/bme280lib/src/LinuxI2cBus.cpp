#ifdef __linux__
#include "LinuxI2cBus.hpp"

#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

#include <limits>

namespace bme280 {

LinuxI2cBus::LinuxI2cBus(const char* device, uint8_t address) : address_(address)
{
    fd_ = ::open(device, O_RDWR);
}

LinuxI2cBus::~LinuxI2cBus()
{
    if (fd_ >= 0) ::close(fd_);
}

bool LinuxI2cBus::read(uint8_t reg, uint8_t* data, size_t len)
{
    if (fd_ < 0 || len > std::numeric_limits<uint16_t>::max()) return false;

    // Two messages, one transaction: repeated start between them.
    i2c_msg msgs[2] = {};
    msgs[0].addr = address_; msgs[0].flags = 0;        msgs[0].len = 1;                          msgs[0].buf = &reg;
    msgs[1].addr = address_; msgs[1].flags = I2C_M_RD; msgs[1].len = static_cast<uint16_t>(len); msgs[1].buf = data;

    i2c_rdwr_ioctl_data xfer = {};
    xfer.msgs  = msgs;
    xfer.nmsgs = 2;
    return ::ioctl(fd_, I2C_RDWR, &xfer) >= 0;
}

bool LinuxI2cBus::write(uint8_t reg, uint8_t value)
{
    if (fd_ < 0) return false;

    uint8_t buf[2] = { reg, value };
    i2c_msg msg = {};
    msg.addr = address_; msg.flags = 0; msg.len = 2; msg.buf = buf;

    i2c_rdwr_ioctl_data xfer = {};
    xfer.msgs  = &msg;
    xfer.nmsgs = 1;
    return ::ioctl(fd_, I2C_RDWR, &xfer) >= 0;
}

void LinuxClock::delayUs(uint32_t us)
{
    timespec ts = {};
    ts.tv_sec  = us / 1000000U;
    ts.tv_nsec = static_cast<long>(us % 1000000U) * 1000L;
    while (::nanosleep(&ts, &ts) != 0) { /* interrupted: ts holds the remainder */ }
}

} // namespace bme280
#endif // __linux__
