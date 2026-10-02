// BME280 on a Raspberry Pi over /dev/i2c-1. Same sensor class as the Arduino
// examples; only the two platform objects differ.
//
//   i2cdetect -y 1        # 76 or 77
//   ./bme280_rpi [addr]   # default 0x76

#include "Bme280.hpp"
#include "LinuxI2cBus.hpp"

#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <thread>

namespace {
std::atomic<bool> running{true};
void onSigint(int) { running = false; }
} // namespace

int main(int argc, char** argv)
{
    const auto address = static_cast<uint8_t>(argc > 1 ? std::strtoul(argv[1], nullptr, 0) : 0x76);
    std::signal(SIGINT, onSigint);

    bme280::LinuxI2cBus bus("/dev/i2c-1", address);   // owns the fd until it goes out of scope
    bme280::LinuxClock  sysClock;
    bme280::Bme280      sensor(bus, sysClock);

    if (!bus.isOpen()) {
        std::fprintf(stderr, "cannot open /dev/i2c-1 (raspi-config -> I2C enabled? user in group i2c?)\n");
        return 1;
    }
    if (const auto err = sensor.init(); err != bme280::Error::None) {
        std::fprintf(stderr, "init failed: error %d at 0x%02X\n", static_cast<int>(err), address);
        return 2;
    }
    std::printf("BME280 ready at 0x%02X, measurement %u us\n", address, sensor.measurementTimeUs());

    while (running) {
        bme280::Measurement m;
        if (sensor.readForced(m) == bme280::Error::None) {
            std::printf("T = %6.2f C   p = %8.1f Pa   RH = %5.1f %%\n", m.temperatureC, m.pressurePa, m.humidityPct);
        } else {
            std::printf("read failed\n");
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::printf("\nbye\n");
    return 0;   // ~Bme280, then ~LinuxI2cBus closes the descriptor
}
