// Smoke test for the sensor library on a Raspberry Pi.
//
//   i2cdetect -y 1            # sensor should show up at 0x76 or 0x77
//   ./rpi_smoke_test [addr]   # default 0x76
//
// Prints one measurement per second until Ctrl-C.

#include "bme280/Bme280.hpp"
#include "rpi/LinuxI2cBus.hpp"

#include <atomic>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <thread>

namespace {
std::atomic<bool> keepRunning{true};
void onSigint(int) { keepRunning = false; }

const char* toString(bme280::Error e)
{
    switch (e) {
        case bme280::Error::None:           return "None";
        case bme280::Error::NotInitialised: return "NotInitialised";
        case bme280::Error::BusFailure:     return "BusFailure";
        case bme280::Error::WrongChipId:    return "WrongChipId";
        case bme280::Error::InvalidConfig:  return "InvalidConfig";
        default:                            return "Unknown";
    }
}
} // namespace

int main(int argc, char** argv)
{
    const auto address = static_cast<uint8_t>(argc > 1 ? std::strtoul(argv[1], nullptr, 0) : 0x76);
    std::signal(SIGINT, onSigint);

    // --- composition root: the only place that knows about Linux -----------
    rpi::LinuxI2cBus bus("/dev/i2c-1", address);
    if (!bus.isOpen()) {
        std::fprintf(stderr, "cannot open /dev/i2c-1 (is I2C enabled in raspi-config? are you in group i2c?)\n");
        return 1;
    }

    bme280::Bme280 sensor(bus);           // week 4 step 3: Bme280 sensor(bus, clock);
    if (const auto err = sensor.init(); err != bme280::Error::None) {
        std::fprintf(stderr, "init failed: %s (address 0x%02X)\n", toString(err), address);
        return 2;
    }
    std::printf("sensor ready at 0x%02X, measurement takes %u us\n", address, sensor.measurementTimeUs());

    // --- main loop -----------------------------------------------------------
    while (keepRunning) {
        bme280::Measurement m;
        if (const auto err = sensor.readForced(m); err == bme280::Error::None) {
            std::printf("T = %6.2f C   p = %9.1f Pa   RH = %5.1f %%\n", m.temperatureC, m.pressurePa, m.humidityPct);
        } else {
            std::printf("read failed: %s\n", toString(err));
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::printf("\nbye\n");
    return 0;   // ~Bme280, then ~LinuxI2cBus closes /dev/i2c-1: RAII
}
