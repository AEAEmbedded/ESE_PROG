// BME280 on a SparkFun MicroMod STM32F405 (ATP carrier), STM32duino core.
//
// ATP carrier headers, STM32 pins, peripherals (from the board's Zephyr dts):
//   "I2C"  SCL/SDA   = PB10/PB11 = I2C2   (also the Qwiic connector)
//   SCL1/SDA1        = PB6/PB7   = I2C1   <- the sensor is here in this setup
// Pins are given explicitly: what `Wire` means depends on the board you selected
// in the IDE (generic F405: PB7/PB6, SparkFun MicroMod: PB11/PB10).
// The sketch owns the bus: it creates it, begins it, sets the clock.
// The sensor only gets a reference to it.

#include <Bme280.hpp>
#include <ArduinoI2cBus.hpp>

TwoWire sensorBus(PB7, PB6);                       // I2C1 = SDA1/SCL1 header. Qwiic/"I2C" header: TwoWire(PB11, PB10)

bme280::ArduinoI2cBus bus(sensorBus, 0x77);        // SDO high on this breakout; 0x76 when SDO is low
bme280::ArduinoClock  sysClock;
bme280::Bme280        sensor(bus, sysClock);          // no hardware touched yet

void setup()
{
    Serial.begin(115200);
    while (!Serial && millis() < 3000) {}

    sensorBus.begin();
    sensorBus.setClock(400000);

    const bme280::Error err = sensor.init();       // reset, chip id, calibration, config
    if (err != bme280::Error::None) {
        Serial.print("BME280 init failed, error ");
        Serial.println(static_cast<int>(err));
        while (true) { delay(1000); }
    }
    Serial.print("BME280 ready, one measurement takes ");
    Serial.print(sensor.measurementTimeUs());
    Serial.println(" us");
}

void loop()
{
    bme280::Measurement m;
    if (sensor.readForced(m) == bme280::Error::None) {
        Serial.print("T = ");  Serial.print(m.temperatureC, 2); Serial.print(" C   ");
        Serial.print("p = ");  Serial.print(m.pressurePa, 0);   Serial.print(" Pa   ");
        Serial.print("RH = "); Serial.print(m.humidityPct, 1);  Serial.println(" %");
    } else {
        Serial.println("read failed");
    }
    delay(1000);
}
