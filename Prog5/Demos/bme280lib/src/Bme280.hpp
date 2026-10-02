#pragma once

#include "Bus.hpp"
#include "Clock.hpp"
#include "Types.hpp"

namespace bme280 {

/// Driver for the Bosch BME280 temperature / pressure / humidity sensor.
///
/// Construction is free and touches no hardware; init() does. Every method
/// reports through Error. No heap, no exceptions, no floating point inside:
/// compensation is the datasheet's integer arithmetic (section 4.2.3).
///
/// Lifecycle: Bme280(bus, clock) -> init() -> readForced() ... (any number)
class Bme280 {
public:
    static constexpr uint8_t kChipId = 0x60;

    // The parameter is called `clock` on purpose: it is the bme280::Clock, and a
    // parameter may share its name with C's clock() without a warning. A *variable*
    // in a sketch may not: name those `sysClock` (see examples/).
    Bme280(Bus& bus, Clock& clock) : bus_(bus), clock_(clock) {}

    Bme280(const Bme280&)            = delete;   // one object per physical sensor
    Bme280& operator=(const Bme280&) = delete;

    /// Soft reset, verify chip ID, load calibration, apply `config`.
    [[nodiscard]] Error init(const Config& config = Config{});

    /// Re-apply a configuration after init().
    [[nodiscard]] Error configure(const Config& config);

    /// Trigger one measurement, wait for it, read and compensate it.
    [[nodiscard]] Error readForced(RawMeasurement& out);
    [[nodiscard]] Error readForced(Measurement& out);

    /// Read the most recent sample without triggering (Mode::Normal).
    [[nodiscard]] Error read(RawMeasurement& out);
    [[nodiscard]] Error read(Measurement& out);

    /// Worst-case duration of one measurement with the current config
    /// (datasheet appendix B), in microseconds.
    [[nodiscard]] uint32_t measurementTimeUs() const;

    [[nodiscard]] bool isInitialised() const { return initialised_; }

private:
    struct Calibration {
        uint16_t t1; int16_t t2, t3;
        uint16_t p1; int16_t p2, p3, p4, p5, p6, p7, p8, p9;
        uint8_t  h1; int16_t h2; uint8_t h3; int16_t h4, h5; int8_t h6;
    };

    [[nodiscard]] Error readCalibration();
    [[nodiscard]] Error waitUntilIdle();
    [[nodiscard]] Error readRaw(int32_t& adcT, int32_t& adcP, int32_t& adcH);

    // Pure functions of calibration + ADC value; tFine links them.
    int32_t  compensateTemperature(int32_t adcT, int32_t& tFine) const;
    uint32_t compensatePressure(int32_t adcP, int32_t tFine) const;
    uint32_t compensateHumidity(int32_t adcH, int32_t tFine) const;

    Bus&        bus_;
    Clock&      clock_;
    Calibration cal_{};
    Config      config_{};
    bool        initialised_ = false;
};

} // namespace bme280
