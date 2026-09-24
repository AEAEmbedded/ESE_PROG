#include "bme280/Bme280.hpp"

namespace bme280 {

// ---------------------------------------------------------------- bridge --
// The only place where C and C++ meet. Static members have no `this`, so
// they are valid C function pointers; `intf` carries the Bus back to us.

int8_t Bme280::readCb(uint8_t reg, uint8_t* data, uint32_t len, void* intf)
{
    auto* bus = static_cast<Bus*>(intf);
    return bus->read(reg, data, len) ? BME280_OK : BME280_E_COMM_FAIL;
}

int8_t Bme280::writeCb(uint8_t reg, const uint8_t* data, uint32_t len, void* intf)
{
    auto* bus = static_cast<Bus*>(intf);
    return bus->write(reg, data, len) ? BME280_OK : BME280_E_COMM_FAIL;
}

void Bme280::delayCb(uint32_t us, void* intf)
{
    static_cast<Bus*>(intf)->delayUs(us);
}

// ----------------------------------------------------------- translation --

Error Bme280::toError(int8_t bosch_result)
{
    switch (bosch_result) {
        case BME280_OK:                 return Error::None;
        case BME280_E_COMM_FAIL:        return Error::BusFailure;
        case BME280_E_DEV_NOT_FOUND:    return Error::WrongChipId;
        case BME280_E_INVALID_LEN:      return Error::InvalidConfig;
        case BME280_E_SLEEP_MODE_FAIL:  return Error::InvalidConfig;
        default:                        return Error::Unknown;
    }
}

void Bme280::toBoschSettings(const Config& in, bme280_settings& out)
{
    // TODO: map each enum onto BME280_OVERSAMPLING_*, BME280_FILTER_COEFF_*
    //       and BME280_STANDBY_TIME_* from bme280_defs.h (osr_t, osr_p,
    //       osr_h, filter, standby_time). Keep the whole mapping here.
    (void)in;
    (void)out;
}

// ------------------------------------------------------------- lifecycle --

Bme280::Bme280(Bus& bus) : bus_(bus)
{
    dev_.intf     = BME280_I2C_INTF;   // TODO: make selectable if SPI is needed
    dev_.intf_ptr = &bus_;
    dev_.read     = &Bme280::readCb;
    dev_.write    = &Bme280::writeCb;
    dev_.delay_us = &Bme280::delayCb;
}

Error Bme280::init(const Config& config)
{
    // TODO: bme280_init(&dev_)  -> soft reset, checks chip ID (0x60), reads calibration
    //       on success: initialised_ = true; return configure(config);
    (void)config;
    return Error::Unknown;
}

Error Bme280::configure(const Config& config)
{
    // TODO: toBoschSettings(config, settings_);
    //       bme280_set_sensor_settings(BME280_SEL_ALL_SETTINGS, &settings_, &dev_)
    (void)config;
    return Error::Unknown;
}

Error Bme280::setMode(Mode mode)
{
    // TODO: bme280_set_sensor_mode(BME280_POWERMODE_*, &dev_)
    (void)mode;
    return Error::Unknown;
}

Error Bme280::readForced(Measurement& out)
{
    // TODO: setMode(Mode::Forced); bus_.delayUs(measurementTimeUs()); read(out)
    (void)out;
    return Error::Unknown;
}

Error Bme280::read(Measurement& out)
{
    if (!initialised_) {
        return Error::NotInitialised;
    }
    // TODO: bme280_data data; bme280_get_sensor_data(BME280_ALL, &data, &dev_);
    //       BME280_32BIT_ENABLE units: temperature 0.01 degC, pressure Pa,
    //       humidity 1/1024 %RH -> convert into `out`.
    (void)out;
    return Error::Unknown;
}

uint32_t Bme280::measurementTimeUs() const
{
    // TODO: uint32_t us; bme280_cal_meas_delay(&us, &settings_); return us;
    return 0;
}

} // namespace bme280
