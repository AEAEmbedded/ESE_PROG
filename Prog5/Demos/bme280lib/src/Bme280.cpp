#include "Bme280.hpp"

namespace bme280 {

namespace reg {
constexpr uint8_t kCalib00  = 0x88;  // 26 bytes: T1..T3, P1..P9, (0xA0 unused), H1
constexpr uint8_t kId       = 0xD0;
constexpr uint8_t kReset    = 0xE0;
constexpr uint8_t kCalib26  = 0xE1;  // 7 bytes: H2..H6
constexpr uint8_t kCtrlHum  = 0xF2;
constexpr uint8_t kStatus   = 0xF3;
constexpr uint8_t kCtrlMeas = 0xF4;
constexpr uint8_t kConfig   = 0xF5;
constexpr uint8_t kData     = 0xF7;  // 8 bytes: press[3], temp[3], hum[2]
} // namespace reg

namespace {
constexpr uint8_t  kResetCommand      = 0xB6;
constexpr uint8_t  kStatusMeasuring   = 1U << 3;
constexpr uint8_t  kStatusImUpdate    = 1U << 0;
constexpr uint32_t kResetSettleUs     = 2000;
constexpr uint8_t  kMaxBusyPolls      = 10;

constexpr uint16_t u16le(const uint8_t* p) { return static_cast<uint16_t>(p[0] | (p[1] << 8)); }
constexpr int16_t  s16le(const uint8_t* p) { return static_cast<int16_t>(u16le(p)); }

/// Oversampling enum -> multiplier used in the timing formula (0 when skipped).
constexpr uint32_t factor(Oversampling os)
{
    return os == Oversampling::Skip ? 0U : (1U << (static_cast<uint8_t>(os) - 1));
}
} // namespace

// ------------------------------------------------------------------ public --

Error Bme280::init(const Config& config)
{
    initialised_ = false;

    uint8_t id = 0;
    if (!bus_.read(reg::kId, &id, 1))  return Error::BusFailure;
    if (id != kChipId)                  return Error::WrongChipId;

    if (!bus_.write(reg::kReset, kResetCommand)) return Error::BusFailure;
    clock_.delayUs(kResetSettleUs);

    // After reset the sensor copies NVM calibration into registers (im_update).
    if (const Error e = waitUntilIdle(); e != Error::None) return e;
    if (const Error e = readCalibration(); e != Error::None) return e;

    initialised_ = true;
    return configure(config);
}

Error Bme280::configure(const Config& config)
{
    if (!initialised_) return Error::NotInitialised;

    // Order matters: ctrl_hum only takes effect after a write to ctrl_meas.
    const auto ctrlHum  = static_cast<uint8_t>(config.humidity);
    const auto cfg      = static_cast<uint8_t>((static_cast<uint8_t>(config.standby) << 5) |
                                               (static_cast<uint8_t>(config.filter) << 2));
    const auto ctrlMeas = static_cast<uint8_t>((static_cast<uint8_t>(config.temperature) << 5) |
                                               (static_cast<uint8_t>(config.pressure) << 2) |
                                               static_cast<uint8_t>(Mode::Sleep));

    if (!bus_.write(reg::kCtrlHum, ctrlHum))   return Error::BusFailure;
    if (!bus_.write(reg::kConfig, cfg))        return Error::BusFailure;   // writable in sleep only
    if (!bus_.write(reg::kCtrlMeas, ctrlMeas)) return Error::BusFailure;

    config_ = config;
    return Error::None;
}

Error Bme280::readForced(RawMeasurement& out)
{
    if (!initialised_) return Error::NotInitialised;

    const auto ctrlMeas = static_cast<uint8_t>((static_cast<uint8_t>(config_.temperature) << 5) |
                                               (static_cast<uint8_t>(config_.pressure) << 2) |
                                               static_cast<uint8_t>(Mode::Forced));
    if (!bus_.write(reg::kCtrlMeas, ctrlMeas)) return Error::BusFailure;

    clock_.delayUs(measurementTimeUs());
    if (const Error e = waitUntilIdle(); e != Error::None) return e;

    return read(out);
}

Error Bme280::readForced(Measurement& out)
{
    RawMeasurement raw{};
    const Error e = readForced(raw);
    if (e == Error::None) out = Measurement::from(raw);
    return e;
}

Error Bme280::read(RawMeasurement& out)
{
    if (!initialised_) return Error::NotInitialised;

    int32_t adcT = 0, adcP = 0, adcH = 0;
    if (const Error e = readRaw(adcT, adcP, adcH); e != Error::None) return e;

    int32_t tFine = 0;
    out.temperatureCenti = compensateTemperature(adcT, tFine);
    out.pressureQ24_8    = compensatePressure(adcP, tFine);
    out.humidityQ22_10   = compensateHumidity(adcH, tFine);
    return Error::None;
}

Error Bme280::read(Measurement& out)
{
    RawMeasurement raw{};
    const Error e = read(raw);
    if (e == Error::None) out = Measurement::from(raw);
    return e;
}

uint32_t Bme280::measurementTimeUs() const
{
    // Datasheet appendix B, t_measure,max, in microseconds.
    uint32_t us = 1250;
    if (config_.temperature != Oversampling::Skip) us += 2300 * factor(config_.temperature);
    if (config_.pressure    != Oversampling::Skip) us += 2300 * factor(config_.pressure) + 575;
    if (config_.humidity    != Oversampling::Skip) us += 2300 * factor(config_.humidity) + 575;
    return us;
}

// ----------------------------------------------------------------- private --

Error Bme280::waitUntilIdle()
{
    for (uint8_t i = 0; i < kMaxBusyPolls; ++i) {
        uint8_t status = 0;
        if (!bus_.read(reg::kStatus, &status, 1)) return Error::BusFailure;
        if ((status & (kStatusMeasuring | kStatusImUpdate)) == 0) return Error::None;
        clock_.delayUs(1000);
    }
    return Error::Timeout;
}

Error Bme280::readCalibration()
{
    uint8_t a[26] = {};
    uint8_t b[7]  = {};
    if (!bus_.read(reg::kCalib00, a, sizeof a)) return Error::BusFailure;
    if (!bus_.read(reg::kCalib26, b, sizeof b)) return Error::BusFailure;

    cal_.t1 = u16le(a + 0);  cal_.t2 = s16le(a + 2);  cal_.t3 = s16le(a + 4);
    cal_.p1 = u16le(a + 6);  cal_.p2 = s16le(a + 8);  cal_.p3 = s16le(a + 10);
    cal_.p4 = s16le(a + 12); cal_.p5 = s16le(a + 14); cal_.p6 = s16le(a + 16);
    cal_.p7 = s16le(a + 18); cal_.p8 = s16le(a + 20); cal_.p9 = s16le(a + 22);
    cal_.h1 = a[25];
    cal_.h2 = s16le(b + 0);
    cal_.h3 = b[2];
    cal_.h4 = static_cast<int16_t>((static_cast<int16_t>(static_cast<int8_t>(b[3])) << 4) | (b[4] & 0x0F));
    cal_.h5 = static_cast<int16_t>((static_cast<int16_t>(static_cast<int8_t>(b[5])) << 4) | (b[4] >> 4));
    cal_.h6 = static_cast<int8_t>(b[6]);
    return Error::None;
}

Error Bme280::readRaw(int32_t& adcT, int32_t& adcP, int32_t& adcH)
{
    uint8_t d[8] = {};   // one burst: press msb/lsb/xlsb, temp msb/lsb/xlsb, hum msb/lsb
    if (!bus_.read(reg::kData, d, sizeof d)) return Error::BusFailure;
    adcP = static_cast<int32_t>((static_cast<uint32_t>(d[0]) << 12) | (static_cast<uint32_t>(d[1]) << 4) | (d[2] >> 4));
    adcT = static_cast<int32_t>((static_cast<uint32_t>(d[3]) << 12) | (static_cast<uint32_t>(d[4]) << 4) | (d[5] >> 4));
    adcH = static_cast<int32_t>((static_cast<uint32_t>(d[6]) << 8)  |  d[7]);
    return Error::None;
}

// Datasheet 4.2.3, verbatim integer formulas. Kept ugly on purpose: they are
// the reference, and the unit test pins them to the datasheet example.

int32_t Bme280::compensateTemperature(int32_t adcT, int32_t& tFine) const
{
    const int32_t var1 = ((((adcT >> 3) - (static_cast<int32_t>(cal_.t1) << 1))) * static_cast<int32_t>(cal_.t2)) >> 11;
    const int32_t var2 = (((((adcT >> 4) - static_cast<int32_t>(cal_.t1)) * ((adcT >> 4) - static_cast<int32_t>(cal_.t1))) >> 12) *
                          static_cast<int32_t>(cal_.t3)) >> 14;
    tFine = var1 + var2;
    return (tFine * 5 + 128) >> 8;
}

uint32_t Bme280::compensatePressure(int32_t adcP, int32_t tFine) const
{
    int64_t var1 = static_cast<int64_t>(tFine) - 128000;
    int64_t var2 = var1 * var1 * static_cast<int64_t>(cal_.p6);
    var2 = var2 + ((var1 * static_cast<int64_t>(cal_.p5)) << 17);
    var2 = var2 + (static_cast<int64_t>(cal_.p4) << 35);
    var1 = ((var1 * var1 * static_cast<int64_t>(cal_.p3)) >> 8) + ((var1 * static_cast<int64_t>(cal_.p2)) << 12);
    var1 = ((((static_cast<int64_t>(1)) << 47) + var1)) * static_cast<int64_t>(cal_.p1) >> 33;
    if (var1 == 0) return 0;   // avoid division by zero
    int64_t p = 1048576 - adcP;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (static_cast<int64_t>(cal_.p9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (static_cast<int64_t>(cal_.p8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (static_cast<int64_t>(cal_.p7) << 4);
    return static_cast<uint32_t>(p);
}

uint32_t Bme280::compensateHumidity(int32_t adcH, int32_t tFine) const
{
    int32_t v = tFine - 76800;
    v = (((((adcH << 14) - (static_cast<int32_t>(cal_.h4) << 20) - (static_cast<int32_t>(cal_.h5) * v)) + 16384) >> 15) *
         (((((((v * static_cast<int32_t>(cal_.h6)) >> 10) * (((v * static_cast<int32_t>(cal_.h3)) >> 11) + 32768)) >> 10) + 2097152) *
           static_cast<int32_t>(cal_.h2) + 8192) >> 14));
    v = v - (((((v >> 15) * (v >> 15)) >> 7) * static_cast<int32_t>(cal_.h1)) >> 4);
    v = v < 0 ? 0 : v;
    v = v > 419430400 ? 419430400 : v;
    return static_cast<uint32_t>(v >> 12);
}

} // namespace bme280
