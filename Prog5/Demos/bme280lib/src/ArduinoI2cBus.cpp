#ifdef ARDUINO

#include "ArduinoI2cBus.hpp"

namespace bme280 {

/// Register write, repeated start, read: one I2C transaction, as the
/// datasheet draws it. Verified on STM32duino 3.x (MicroMod F405, I2C1).
/// useRepeatedStart(false) inserts a STOP instead; the BME280 keeps its
/// register pointer across it, for cores whose Wire cannot do a repeated start.
bool ArduinoI2cBus::read(uint8_t reg, uint8_t* data, size_t len)
{
    wire_.beginTransmission(address_);
    wire_.write(reg);
    if (wire_.endTransmission(!repeatedStart_) != 0) return false;
    if (wire_.requestFrom(address_, static_cast<uint8_t>(len)) != len) return false;
    for (size_t i = 0; i < len; ++i) data[i] = static_cast<uint8_t>(wire_.read());
    return true;
}

bool ArduinoI2cBus::write(uint8_t reg, uint8_t value)
{
    wire_.beginTransmission(address_);
    wire_.write(reg);
    wire_.write(value);
    return wire_.endTransmission() == 0;
}

} // namespace bme280

#endif // ARDUINO
