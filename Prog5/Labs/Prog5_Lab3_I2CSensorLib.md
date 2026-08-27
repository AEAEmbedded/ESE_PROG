## Class diagram Sensor Library (ToF)

The following class diagram is used:

```mermaid

classDiagram
class I2CHelper {
    +I2CHelper(TwoWire* wire)
    +template <typename T> T readRegister(uint8_t address, uint16_t reg)
    +template <typename T> void writeRegister(uint8_t address, uint16_t reg, T value)
    -TwoWire* _wire
}

class VL6180X {
    +VL6180X(uint8_t address = 0x29, I2CHelper i2cHelper = I2CHelper(&Wire))
    +bool begin()
    +bool init()
    +bool configureDefault()
    +uint8_t readRangeSingle()
    +uint8_t readRangeContinuous()
    +uint8_t getModelId()
    +void startSingleRangeMeasurement()
    +bool isDataReady()
    +uint8_t getRange()
    +void clearInterrupt()
    -uint8_t _address
    -I2CHelper _i2cHelper
    -static const uint8_t DEFAULT_SENSOR_ADDRESS
    -static const uint16_t IDENTIFICATION__MODEL_ID
    -static const uint16_t SYSRANGE__START
    -static const uint16_t ... (other register addresses)
}

VL6180X --> I2CHelper : uses
