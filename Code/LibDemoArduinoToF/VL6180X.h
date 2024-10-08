/**
 * @file VL6180X.h
 * @brief Arduino library for the VL6180X Time-of-Flight distance sensor.
 * @author J.A. Korten / johan.korten@han.nl
 * @version 1.0
 * @date 2024-09-16 
 */

#ifndef VL6180X_h
#define VL6180X_h

#include "Arduino.h"
#include "I2CHelper.h"

#define VL6180x_FAILURE_RESET -1

#define VL6180X_IDENTIFICATION_MODEL_ID 0x0000
#define VL6180X_IDENTIFICATION_MODEL_REV_MAJOR 0x0001
#define VL6180X_IDENTIFICATION_MODEL_REV_MINOR 0x0002
#define VL6180X_IDENTIFICATION_MODULE_REV_MAJOR 0x0003
#define VL6180X_IDENTIFICATION_MODULE_REV_MINOR 0x0004
#define VL6180X_IDENTIFICATION_DATE 0x0006 // 16bit value
#define VL6180X_IDENTIFICATION_TIME 0x0008 // 16bit value

#define VL6180X_SYSTEM_MODE_GPIO0 0x0010
#define VL6180X_SYSTEM_MODE_GPIO1 0x0011
#define VL6180X_SYSTEM_HISTORY_CTRL 0x0012
#define VL6180X_SYSTEM_INTERRUPT_CONFIG_GPIO 0x0014
#define VL6180X_SYSTEM_INTERRUPT_CLEAR 0x0015
#define VL6180X_SYSTEM_FRESH_OUT_OF_RESET 0x0016
#define VL6180X_SYSTEM_GROUPED_PARAMETER_HOLD 0x0017

#define VL6180X_SYSRANGE_START 0x0018
#define VL6180X_SYSRANGE_THRESH_HIGH 0x0019
#define VL6180X_SYSRANGE_THRESH_LOW 0x001A
#define VL6180X_SYSRANGE_INTERMEASUREMENT_PERIOD 0x001B
#define VL6180X_SYSRANGE_MAX_CONVERGENCE_TIME 0x001C
#define VL6180X_SYSRANGE_CROSSTALK_COMPENSATION_RATE 0x001E
#define VL6180X_SYSRANGE_CROSSTALK_VALID_HEIGHT 0x0021
#define VL6180X_SYSRANGE_EARLY_CONVERGENCE_ESTIMATE 0x0022
#define VL6180X_SYSRANGE_PART_TO_PART_RANGE_OFFSET 0x0024
#define VL6180X_SYSRANGE_RANGE_IGNORE_VALID_HEIGHT 0x0025
#define VL6180X_SYSRANGE_RANGE_IGNORE_THRESHOLD 0x0026
#define VL6180X_SYSRANGE_MAX_AMBIENT_LEVEL_MULT 0x002C
#define VL6180X_SYSRANGE_RANGE_CHECK_ENABLES 0x002D
#define VL6180X_SYSRANGE_VHV_RECALIBRATE 0x002E
#define VL6180X_SYSRANGE_VHV_REPEAT_RATE 0x0031

#define VL6180X_SYSALS_START 0x0038
#define VL6180X_SYSALS_THRESH_HIGH 0x003A
#define VL6180X_SYSALS_THRESH_LOW 0x003C
#define VL6180X_SYSALS_INTERMEASUREMENT_PERIOD 0x003E
#define VL6180X_SYSALS_ANALOGUE_GAIN 0x003F
#define VL6180X_SYSALS_INTEGRATION_PERIOD 0x0040

#define VL6180X_RESULT_RANGE_STATUS 0x004D
#define VL6180X_RESULT_ALS_STATUS 0x004E
#define VL6180X_RESULT_INTERRUPT_STATUS_GPIO 0x004F
#define VL6180X_RESULT_ALS_VAL 0x0050
#define VL6180X_RESULT_HISTORY_BUFFER 0x0052
#define VL6180X_RESULT_RANGE_VAL 0x0062
#define VL6180X_RESULT_RANGE_RAW 0x0064
#define VL6180X_RESULT_RANGE_RETURN_RATE 0x0066
#define VL6180X_RESULT_RANGE_REFERENCE_RATE 0x0068
#define VL6180X_RESULT_RANGE_RETURN_SIGNAL_COUNT 0x006C
#define VL6180X_RESULT_RANGE_REFERENCE_SIGNAL_COUNT 0x0070
#define VL6180X_RESULT_RANGE_RETURN_AMB_COUNT 0x0074
#define VL6180X_RESULT_RANGE_REFERENCE_AMB_COUNT 0x0078
#define VL6180X_RESULT_RANGE_RETURN_CONV_TIME 0x007C
#define VL6180X_RESULT_RANGE_REFERENCE_CONV_TIME 0x0080

#define VL6180X_READOUT_AVERAGING_SAMPLE_PERIOD 0x010A
#define VL6180X_FIRMWARE_BOOTUP 0x0119
#define VL6180X_FIRMWARE_RESULT_SCALER 0x0120
#define VL6180X_I2C_SLAVE_DEVICE_ADDRESS 0x0212
#define VL6180X_INTERLEAVED_MODE_ENABLE 0x02A3

enum vl6180x_als_gain
{ // Data sheet shows gain values as binary list

  GAIN_20 = 0, // Actual ALS Gain of 20
  GAIN_10,     // Actual ALS Gain of 10.32
  GAIN_5,      // Actual ALS Gain of 5.21
  GAIN_2_5,    // Actual ALS Gain of 2.60
  GAIN_1_67,   // Actual ALS Gain of 1.72
  GAIN_1_25,   // Actual ALS Gain of 1.28
  GAIN_1,      // Actual ALS Gain of 1.01
  GAIN_40,     // Actual ALS Gain of 40

};

struct VL6180xIdentification
{
  uint8_t idModel;
  uint8_t idModelRevMajor;
  uint8_t idModelRevMinor;
  uint8_t idModuleRevMajor;
  uint8_t idModuleRevMinor;
  uint16_t idDate;
  uint16_t idTime;
};



class VL6180X {
public:
  // Define the default sensor address as a constant
  static const uint8_t DEFAULT_SENSOR_ADDRESS = 0x29;

  // Constructor with optional I2C address and I2CHelper instance
  VL6180X(uint8_t address = DEFAULT_SENSOR_ADDRESS, I2CHelper i2cHelper = I2CHelper(&Wire));

  bool begin();
  bool init();
  bool configureDefault();

  uint8_t readRangeSingle();
  uint8_t readRangeContinuous();

  // Convenience methods for common operations
  uint8_t getModelId();
  void startSingleRangeMeasurement();
  bool isDataReady();
  uint8_t getRange();
  void clearInterrupt();

private:
  uint8_t _address;
  I2CHelper _i2cHelper;  // Store the I2CHelper object directly

  // Constants for register addresses (consider using enums for clarity)
  static const uint16_t IDENTIFICATION__MODEL_ID = 0x0000;
  static const uint16_t SYSRANGE__START = 0x0018;
  static const uint16_t SYSRANGE__THRESH_HIGH = 0x0019;
  static const uint16_t SYSRANGE__THRESH_LOW = 0x001A;
  static const uint16_t SYSRANGE__INTERMEASUREMENT_PERIOD = 0x001B;
  static const uint16_t SYSRANGE__MAX_CONVERGENCE_TIME = 0x001C;
  static const uint16_t SYSRANGE__VHV_REPEAT = 0x0021;
  static const uint16_t RESULT__RANGE_STATUS = 0x004D;
  static const uint16_t RESULT__INTERRUPT_STATUS_GPIO = 0x004F;
  static const uint16_t RESULT__RANGE_VAL = 0x0062;
  static const uint16_t SYSTEM__INTERRUPT_CLEAR = 0x0015;
};

#endif