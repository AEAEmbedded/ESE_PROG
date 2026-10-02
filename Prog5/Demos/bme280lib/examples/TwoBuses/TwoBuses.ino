// Two BME280s at the same address on two buses. Only possible because the
// sensor is handed its bus instead of grabbing the global `Wire`.
// STM32F405 MicroMod: Primary I2C = I2C2 (PB11/PB10), Secondary = I2C1 (Wire).

#include <Bme280.hpp>
#include <ArduinoI2cBus.hpp>

TwoWire primaryI2c(PB11, PB10);    // I2C2, explicit so the board selection does not matter
TwoWire secondaryI2c(PB7, PB6);    // I2C1

bme280::ArduinoClock  sysClock;                       // one clock is enough
bme280::ArduinoI2cBus busA(primaryI2c, 0x76);
bme280::ArduinoI2cBus busB(secondaryI2c, 0x76);
bme280::Bme280        inside(busA, sysClock);
bme280::Bme280        outside(busB, sysClock);

bme280::Bme280* sensors[] = { &inside, &outside };
const char*     names[]   = { "inside", "outside" };

void setup()
{
    Serial.begin(115200);
    primaryI2c.begin();
    secondaryI2c.begin();
    for (auto* s : sensors) {
        if (s->init() != bme280::Error::None) Serial.println("a sensor did not answer");
    }
}

void loop()
{
    for (size_t i = 0; i < 2; ++i) {
        bme280::RawMeasurement r;                  // fixed point: no float on the way
        if (sensors[i]->readForced(r) == bme280::Error::None) {
            Serial.print(names[i]); Serial.print(": ");
            Serial.print(r.temperatureCenti / 100); Serial.print('.');
            Serial.print(r.temperatureCenti % 100); Serial.println(" C");
        }
    }
    delay(1000);
}
