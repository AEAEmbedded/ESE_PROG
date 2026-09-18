#include "rpi/LinuxI2cBus.hpp"

#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

#include <array>
#include <limits>

namespace rpi {

namespace {
// i2c_msg::len is 16 bits; refuse anything we cannot express.
bool fitsInMessage(size_t len)
{
    return len <= std::numeric_limits<uint16_t>::max();
}
} // namespace

LinuxI2cBus::LinuxI2cBus(const char* device, uint8_t address) : address_(address)
{
    fd_ = ::open(device, O_RDWR);
    // Address is passed per message (I2C_RDWR), so no I2C_SLAVE ioctl needed.
    // We still do it so that plain read()/write() on the fd would also work.
    if (fd_ >= 0 && ::ioctl(fd_, I2C_SLAVE, address_) < 0) {
        ::close(fd_);
        fd_ = -1;
    }
}

LinuxI2cBus::~LinuxI2cBus()
{
    if (fd_ >= 0) {
        ::close(fd_);
    }
}

bool LinuxI2cBus::read(uint8_t reg, uint8_t* data, size_t len)
{
    if (fd_ < 0 || !fitsInMessage(len)) {
        return false;
    }

    // Two messages in one transaction -> repeated start between them. The
    // BME280/BMP280 accept a STOP in between as well, but many sensors do not,
    // so this is the version worth copying.
    std::array<i2c_msg, 2> msgs{};
    msgs[0].addr  = address_;
    msgs[0].flags = 0;
    msgs[0].len   = 1;
    msgs[0].buf   = &reg;

    msgs[1].addr  = address_;
    msgs[1].flags = I2C_M_RD;
    msgs[1].len   = static_cast<uint16_t>(len);
    msgs[1].buf   = data;

    i2c_rdwr_ioctl_data xfer{};
    xfer.msgs  = msgs.data();
    xfer.nmsgs = static_cast<uint32_t>(msgs.size());

    return ::ioctl(fd_, I2C_RDWR, &xfer) >= 0;
}

bool LinuxI2cBus::write(uint8_t reg, const uint8_t* data, size_t len)
{
    if (fd_ < 0 || !fitsInMessage(len + 1)) {
        return false;
    }

    // Register address followed by the payload, in one write. The Bosch API
    // interleaves reg/value pairs into `data` itself, so no loop here.
    // Fixed-size buffer: the core never writes more than a handful of bytes,
    // and we do not want heap allocation in a bus driver.
    constexpr size_t kMaxWrite = 32;
    if (len + 1 > kMaxWrite) {
        return false;
    }
    std::array<uint8_t, kMaxWrite> buf{};
    buf[0] = reg;
    for (size_t i = 0; i < len; ++i) {
        buf[i + 1] = data[i];
    }

    i2c_msg msg{};
    msg.addr  = address_;
    msg.flags = 0;
    msg.len   = static_cast<uint16_t>(len + 1);
    msg.buf   = buf.data();

    i2c_rdwr_ioctl_data xfer{};
    xfer.msgs  = &msg;
    xfer.nmsgs = 1;

    return ::ioctl(fd_, I2C_RDWR, &xfer) >= 0;
}

void LinuxI2cBus::delayUs(uint32_t us)
{
    timespec ts{};
    ts.tv_sec  = us / 1000000U;
    ts.tv_nsec = static_cast<long>(us % 1000000U) * 1000L;
    while (::nanosleep(&ts, &ts) != 0) {
        // interrupted by a signal: nanosleep updated ts with the remainder
    }
}

} // namespace rpi
