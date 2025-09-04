/**
 * @file VL6180X.cpp
 * @brief Implementation of the VL6180X Arduino library.
 * @author J.A. Korten / johan.korten@han.nl
 * @version 1.0
 * @date 2024-09-16 
 */

#include "VL6180X.h"

// Constructor
VL6180X::VL6180X(uint8_t address, I2CHelper i2cHelper)
  : _address(address),
    _i2cHelper(i2cHelper) {}

bool VL6180X::begin() {
  // Check sensor ID using the convenience method
  if (getModelId() != 0xB4) {
    return false;
  }
  return true;
}

bool VL6180X::init() {

  uint8_t data;  // for temp data storage

  data = _i2cHelper.readRegister<uint8_t>(_address, VL6180X_SYSTEM_FRESH_OUT_OF_RESET);

  if (data != 1)
    return false; // VL6180x_FAILURE_RESET;

  // Mandatory register settings (refer to datasheet for details)
  _i2cHelper.writeRegister<uint8_t>(_address, 0x0207, 0x01);
  _i2cHelper.writeRegister<uint8_t>(_address, 0x0208, 0x01);
  _i2cHelper.writeRegister<uint8_t>(_address, 0x0096, 0x00);
  _i2cHelper.writeRegister<uint8_t>(_address, 0x0097, 0xfd);
  _i2cHelper.writeRegister<uint8_t>(_address, 0x00e3, 0x00);
  _i2cHelper.writeRegister<uint8_t>(_address, 0x00e4, 0x04);
  _i2cHelper.writeRegister<uint8_t>(_address, 0x00e5, 0x02);
  _i2cHelper.writeRegister<uint8_t>(_address, 0x00e6, 0x01);
  _i2cHelper.writeRegister<uint8_t>(_address, 0x00e7, 0x03);
  _i2cHelper.writeRegister<uint8_t>(_address, 0x00f5, 0x02);
  _i2cHelper.writeRegister<uint8_t>(_address, 0x00d9, 0x05);
  _i2cHelper.writeRegister<uint8_t>(_address, 0x00db, 0xce);
  _i2cHelper.writeRegister<uint8_t>(_address, 0x00dc, 0x03);
  _i2cHelper.writeRegister<uint8_t>(_address, 0x00dd, 0xf8);
  _i2cHelper.writeRegister<uint8_t>(_address, 0x009f, 0x00);
  _i2cHelper.writeRegister<uint8_t>(_address, 0x00a3, 0x3c);
  _i2cHelper.writeRegister<uint8_t>(_address, 0x00b7, 0x00);
  _i2cHelper.writeRegister<uint8_t>(_address, 0x00bb, 0x3c);
  _i2cHelper.writeRegister<uint8_t>(_address, 0x00b2, 0x09);
  _i2cHelper.writeRegister<uint8_t>(_address, 0x00ca, 0x09);
  _i2cHelper.writeRegister<uint8_t>(_address, 0x0198, 0x01);
  _i2cHelper.writeRegister<uint8_t>(_address, 0x01b0, 0x17);
  _i2cHelper.writeRegister<uint8_t>(_address, 0x01ad, 0x00);
  _i2cHelper.writeRegister<uint8_t>(_address, 0x00ff, 0x05);
  _i2cHelper.writeRegister<uint8_t>(_address, 0x0100, 0x05);
  _i2cHelper.writeRegister<uint8_t>(_address, 0x0199, 0x05);
  _i2cHelper.writeRegister<uint8_t>(_address, 0x01a6, 0x1b);
  _i2cHelper.writeRegister<uint8_t>(_address, 0x01ac, 0x3e);
  _i2cHelper.writeRegister<uint8_t>(_address, 0x01a7, 0x1f);
  _i2cHelper.writeRegister<uint8_t>(_address, 0x0030, 0x00);

  return true;
}

bool VL6180X::configureDefault() {
//void VL6180X::VL6180xDefaultSettings() {
    // Recommended settings from datasheet
    // http://www.st.com/st-web-ui/static/active/en/resource/technical/document/application_note/DM00122600.pdf

    // Enable Interrupts on Conversion Complete (any source)   

    _i2cHelper.writeRegister<uint8_t>(_address, VL6180X_SYSTEM_INTERRUPT_CONFIG_GPIO, (4 << 3) | (4)); // Set GPIO1 high when sample complete

    _i2cHelper.writeRegister<uint8_t>(_address, VL6180X_SYSTEM_MODE_GPIO1, 0x10); // Set GPIO1 high when sample complete
    _i2cHelper.writeRegister<uint8_t>(_address, VL6180X_READOUT_AVERAGING_SAMPLE_PERIOD, 0x30); // Set Avg sample period
    _i2cHelper.writeRegister<uint8_t>(_address, VL6180X_SYSALS_ANALOGUE_GAIN, 0x46); // Set the ALS gain
    _i2cHelper.writeRegister<uint8_t>(_address, VL6180X_SYSRANGE_VHV_REPEAT_RATE, 0xFF); // Set auto calibration period (Max = 255)/(OFF = 0)
    _i2cHelper.writeRegister<uint8_t>(_address, VL6180X_SYSALS_INTEGRATION_PERIOD, 0x63); // Set ALS integration time to 100ms
    _i2cHelper.writeRegister<uint8_t>(_address, VL6180X_SYSRANGE_VHV_RECALIBRATE, 0x01); // perform a single temperature calibration

    // Optional settings from datasheet
    // http://www.st.com/st-web-ui/static/active/en/resource/technical/document/application_note/DM00122600.pdf   

    _i2cHelper.writeRegister<uint8_t>(_address, VL6180X_SYSRANGE_INTERMEASUREMENT_PERIOD, 0x09); // Set default ranging inter-measurement period to 100ms
    _i2cHelper.writeRegister<uint8_t>(_address, VL6180X_SYSALS_INTERMEASUREMENT_PERIOD, 0x0A);  // Set default ALS inter-measurement period to 100ms
    _i2cHelper.writeRegister<uint8_t>(_address, VL6180X_SYSTEM_INTERRUPT_CONFIG_GPIO, 0x24);   // Configures interrupt on ‘New Sample Ready threshold event’

    // Additional settings defaults from community   

    _i2cHelper.writeRegister<uint8_t>(_address, VL6180X_SYSRANGE_MAX_CONVERGENCE_TIME, 0x32);
    _i2cHelper.writeRegister<uint8_t>(_address, VL6180X_SYSRANGE_RANGE_CHECK_ENABLES, 0x10 | 0x01);
    _i2cHelper.writeRegister<uint16_t>(_address, VL6180X_SYSRANGE_EARLY_CONVERGENCE_ESTIMATE, 0x7B);
    _i2cHelper.writeRegister<uint16_t>(_address, VL6180X_SYSALS_INTEGRATION_PERIOD, 0x64);

    _i2cHelper.writeRegister<uint8_t>(_address, VL6180X_READOUT_AVERAGING_SAMPLE_PERIOD, 0x30);
    _i2cHelper.writeRegister<uint8_t>(_address, VL6180X_SYSALS_ANALOGUE_GAIN, 0x40);
    _i2cHelper.writeRegister<uint8_t>(_address, VL6180X_FIRMWARE_RESULT_SCALER, 0x01);

    return true;
}

uint8_t VL6180X::readRangeSingle() {
  startSingleRangeMeasurement();

  // Poll for completion using the convenience method
  delay(10);  // Adjust delay based on measurement time
  while (!isDataReady())
    ;

  uint8_t range = getRange();
  clearInterrupt();

  return range;
}

uint8_t VL6180X::readRangeContinuous() {
  _i2cHelper.writeRegister<uint8_t>(_address, SYSRANGE__START, 0x03);  // Start continuous mode

  // Poll for new data ready
  delay(10);  // Adjust delay based on measurement time
  while (!isDataReady())
    ;

  uint8_t range = getRange();
  clearInterrupt();

  return range;
}

uint8_t VL6180X::getModelId() {
  return _i2cHelper.readRegister<uint8_t>(_address, IDENTIFICATION__MODEL_ID);
}

void VL6180X::startSingleRangeMeasurement() {
  _i2cHelper.writeRegister<uint8_t>(_address, SYSRANGE__START, 0x01);
}

bool VL6180X::isDataReady() {
  return (_i2cHelper.readRegister<uint8_t>(_address, RESULT__INTERRUPT_STATUS_GPIO) & 0x07) != 0;
}

uint8_t VL6180X::getRange() {
  return _i2cHelper.readRegister<uint8_t>(_address, RESULT__RANGE_VAL);
}

void VL6180X::clearInterrupt() {
  _i2cHelper.writeRegister<uint8_t>(_address, SYSTEM__INTERRUPT_CLEAR, 0x01);
}

//