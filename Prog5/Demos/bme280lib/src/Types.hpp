#pragma once

#include <cstdint>

namespace bme280 {

/// Every public operation returns one of these. No exceptions, no magic ints.
enum class Error : uint8_t {
    None = 0,
    NotInitialised,  ///< read before a successful init()
    BusFailure,      ///< Bus::read / Bus::write returned false
    WrongChipId,     ///< register 0xD0 did not contain 0x60
    Timeout,         ///< sensor stayed busy longer than expected
};

enum class Oversampling : uint8_t { Skip = 0, X1 = 1, X2 = 2, X4 = 3, X8 = 4, X16 = 5 };
enum class Filter       : uint8_t { Off = 0, Coeff2 = 1, Coeff4 = 2, Coeff8 = 3, Coeff16 = 4 };
enum class Standby      : uint8_t { Ms0_5 = 0, Ms62_5 = 1, Ms125 = 2, Ms250 = 3, Ms500 = 4,
                                    Ms1000 = 5, Ms10 = 6, Ms20 = 7 };
enum class Mode         : uint8_t { Sleep = 0, Forced = 1, Normal = 3 };

/// Datasheet "weather monitoring" defaults: forced mode, x1 / x1 / x1, filter off.
struct Config {
    Oversampling temperature = Oversampling::X1;
    Oversampling pressure    = Oversampling::X1;
    Oversampling humidity    = Oversampling::X1;
    Filter       filter      = Filter::Off;
    Standby      standby     = Standby::Ms1000;  ///< Mode::Normal only
};

/// Fixed-point result, exactly what the compensation maths produces.
/// No float on the way; use this on an MCU without FPU or in a control loop.
struct RawMeasurement {
    int32_t  temperatureCenti;  ///< 0.01 degC   (2508 = 25.08 degC)
    uint32_t pressureQ24_8;     ///< Pa in Q24.8 (value / 256 = Pa)
    uint32_t humidityQ22_10;    ///< %RH in Q22.10 (value / 1024 = %RH)
};

/// Engineering units. Units are in the names so they cannot be misread.
struct Measurement {
    float temperatureC;
    float pressurePa;
    float humidityPct;

    static Measurement from(const RawMeasurement& r)
    {
        return { static_cast<float>(r.temperatureCenti) / 100.0F,
                 static_cast<float>(r.pressureQ24_8) / 256.0F,
                 static_cast<float>(r.humidityQ22_10) / 1024.0F };
    }
};

} // namespace bme280
