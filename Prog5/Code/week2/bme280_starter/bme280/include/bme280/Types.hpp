#pragma once

#include <cstdint>

namespace bme280 {

/// Every public method returns one of these. No exceptions, no negative ints.
enum class Error : uint8_t {
    None = 0,
    NotInitialised,   ///< read() called before init()
    BusFailure,       ///< Bus::read/write returned false
    WrongChipId,      ///< ID register did not contain 0x60
    InvalidConfig,    ///< Bosch API rejected the settings
    Unknown           ///< Bosch API returned a code we did not map
};

/// I2C address depends on the SDO pin level.
enum class I2cAddress : uint8_t { Low = 0x76, High = 0x77 };

enum class Oversampling : uint8_t { Skip, x1, x2, x4, x8, x16 };
enum class Filter : uint8_t { Off, Coeff2, Coeff4, Coeff8, Coeff16 };
enum class Standby : uint8_t { ms0_5, ms62_5, ms125, ms250, ms500, ms1000, ms10, ms20 };
enum class Mode : uint8_t { Sleep, Forced, Normal };

/// Full sensor configuration. Defaults follow the datasheet
/// "weather monitoring" use case (forced mode, x1/x1/x1, filter off).
struct Config {
    Oversampling temperature = Oversampling::x1;
    Oversampling pressure    = Oversampling::x1;
    Oversampling humidity    = Oversampling::x1;
    Filter       filter      = Filter::Off;
    Standby      standby     = Standby::ms1000;  ///< only used in Mode::Normal
};

/// The hand-off to the rest of the weather station (MQTT payload, DB).
/// Units are part of the name so they cannot be misread.
struct Measurement {
    float temperatureC = 0.0F;
    float pressurePa   = 0.0F;
    float humidityPct  = 0.0F;
};

} // namespace bme280
