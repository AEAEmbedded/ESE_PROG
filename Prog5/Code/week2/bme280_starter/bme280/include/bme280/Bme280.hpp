#pragma once

#include "bme280/Bus.hpp"
#include "bme280/Types.hpp"

extern "C" {
#include "bme280.h"   // Bosch BME280 SensorAPI (third_party/bme280)
}

namespace bme280 {

/// C++ wrapper around the Bosch BME280 C driver.
///
/// Lifecycle:  Uninitialised --init()--> Ready --read()--> Ready
///             (any step may return an Error and stay in its state)
///
/// SRP: this class configures and reads one sensor. Byte transport lives in
/// Bus, compensation maths lives in the Bosch code, altitude lives elsewhere.
class Bme280 {
public:
    /// Does not touch hardware; call init() for that.
    explicit Bme280(Bus& bus);

    // Non-copyable: the Bosch struct stores a pointer back to our bus.
    Bme280(const Bme280&)            = delete;
    Bme280& operator=(const Bme280&) = delete;

    /// Soft-reset, verify chip ID, load calibration, apply `config`.
    Error init(const Config& config = Config{});

    /// Re-apply a configuration after init().
    Error configure(const Config& config);

    /// Set power mode. Forced = one measurement then sleep.
    Error setMode(Mode mode);

    /// Trigger a forced measurement, wait, read and compensate.
    Error readForced(Measurement& out);

    /// Read the latest sample (Mode::Normal, or after readForced()).
    Error read(Measurement& out);

    /// Time one measurement takes with the current config, in microseconds.
    uint32_t measurementTimeUs() const;

    bool isInitialised() const { return initialised_; }

private:
    // --- Bridge: C callbacks that forward to Bus via intf_ptr ------------
    static int8_t readCb(uint8_t reg, uint8_t* data, uint32_t len, void* intf);
    static int8_t writeCb(uint8_t reg, const uint8_t* data, uint32_t len, void* intf);
    static void   delayCb(uint32_t us, void* intf);

    static Error toError(int8_t bosch_result);
    static void  toBoschSettings(const Config& in, bme280_settings& out);

    Bus&            bus_;
    bme280_dev      dev_{};
    bme280_settings settings_{};
    bool            initialised_ = false;
};

} // namespace bme280
