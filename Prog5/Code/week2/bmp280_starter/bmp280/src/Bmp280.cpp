#include "bmp280/Bmp280.hpp"

namespace bmp280 {

// ---------------------------------------------------------------- bridge --
// The only place where C and C++ meet. Static members have no `this`, so
// they are valid C function pointers; `intf` carries the Bus back to us.

int8_t Bmp280::readCb(uint8_t reg, uint8_t* data, uint32_t len, void* intf)
{
    auto* bus = static_cast<Bus*>(intf);
    return bus->read(reg, data, len) ? BMP2_OK : BMP2_E_COM_FAIL;
}

int8_t Bmp280::writeCb(uint8_t reg, const uint8_t* data, uint32_t len, void* intf)
{
    auto* bus = static_cast<Bus*>(intf);
    return bus->write(reg, data, len) ? BMP2_OK : BMP2_E_COM_FAIL;
}

void Bmp280::delayCb(uint32_t us, void* intf)
{
    static_cast<Bus*>(intf)->delayUs(us);
}

// ----------------------------------------------------------- translation --

Error Bmp280::toError(int8_t bosch_result)
{
    switch (bosch_result) {
        case BMP2_OK:              return Error::None;
        case BMP2_E_COM_FAIL:      return Error::BusFailure;
        case BMP2_E_DEV_NOT_FOUND: return Error::WrongChipId;
        case BMP2_E_INVALID_LEN:   return Error::InvalidConfig;
        default:                   return Error::Unknown;
    }
}

void Bmp280::toBoschConfig(const Config& in, bmp2_config& out)
{
    // TODO: map each enum onto the BMP2_OS_*, BMP2_FILTER_*, BMP2_ODR_*
    //       constants from bmp2_defs.h. Keep the whole mapping here.
    (void)in;
    (void)out;
}

// ------------------------------------------------------------- lifecycle --

Bmp280::Bmp280(Bus& bus) : bus_(bus)
{
    dev_.intf     = BMP2_I2C_INTF;   // TODO: make selectable if SPI is needed
    dev_.intf_ptr = &bus_;
    dev_.read     = &Bmp280::readCb;
    dev_.write    = &Bmp280::writeCb;
    dev_.delay_us = &Bmp280::delayCb;
}

Error Bmp280::init(const Config& config)
{
    // TODO: bmp2_init(&dev_)  -> checks chip ID, reads calibration
    //       on success: initialised_ = true; return configure(config);
    (void)config;
    return Error::Unknown;
}

Error Bmp280::configure(const Config& config)
{
    // TODO: toBoschConfig(config, config_); bmp2_set_config(&config_, &dev_)
    (void)config;
    return Error::Unknown;
}

Error Bmp280::setMode(Mode mode)
{
    // TODO: bmp2_set_power_mode(BMP2_POWERMODE_*, &config_, &dev_)
    (void)mode;
    return Error::Unknown;
}

Error Bmp280::readForced(Measurement& out)
{
    // TODO: setMode(Mode::Forced); bus_.delayUs(measurementTimeUs()); read(out)
    (void)out;
    return Error::Unknown;
}

Error Bmp280::read(Measurement& out)
{
    if (!initialised_) {
        return Error::NotInitialised;
    }
    // TODO: bmp2_get_sensor_data(&data, &dev_); copy into `out`
    //       (mind the integer/double compensation macro: units differ)
    (void)out;
    return Error::Unknown;
}

uint32_t Bmp280::measurementTimeUs() const
{
    // TODO: bmp2_compute_meas_time(&us, &config_, &dev_)
    return 0;
}

} // namespace bmp280
