/*
    SPO2Sensor.h

    SpO2 Sensor Connection Detection
    Detects if a SpO2 sensor is connected via pull-up resistor detection.

    Hardware:
    - 10k pull-up to 3.3V on SPO2_CONNECTION_A2 pin.
      When sensor is connected, the line is pulled low.
      When sensor is disconnected, the line reads high (3.3V).
    - RED LED output on SPO2_LED_D12 pin for sensor indication.

    Johan Korten
    for HAN ESE / WKZ Hackaton Challenge 2026
*/

#ifndef SPO2_SENSOR_H
#define SPO2_SENSOR_H

#include <Arduino.h>

// Default pin definitions
#define SPO2_CONNECTION_A2 A2
#define SPO2_LED_D12 12

class SPO2Sensor {
public:
    /**
     * Constructor
     * @param pin Analog pin for detection (default: A2)
     * @param ledPin Digital pin for RED LED (default: D12)
     * @param threshold ADC threshold for detection (default: 512, ~1.65V at 10-bit)
     */
    SPO2Sensor(uint8_t pin = SPO2_CONNECTION_A2, uint8_t ledPin = SPO2_LED_D12, uint16_t threshold = 512);

    /**
     * Initialize the sensor detection pin and LED pin
     * Call this in setup()
     */
    void begin();

    /**
     * Update the sensor detection state
     * Call this regularly in loop()
     */
    void update();

    /**
     * Check if SpO2 sensor is connected
     * @return true if sensor is connected (line pulled low)
     */
    bool isConnected() const;

    /**
     * Get the raw ADC value from the detection pin
     * @return 16-bit ADC value
     */
    uint16_t getRawValue() const;

    /**
     * Get the high byte of the raw value
     * @return High byte (bits 8-15)
     */
    uint8_t getHighByte() const;

    /**
     * Get the low byte of the raw value
     * @return Low byte (bits 0-7)
     */
    uint8_t getLowByte() const;

    /**
     * Get connection status as a byte
     * @return 1 if connected, 0 if disconnected
     */
    uint8_t getStatusByte() const;

    /**
     * Set the detection threshold
     * @param threshold ADC value below which sensor is considered connected
     */
    void setThreshold(uint16_t threshold);

    /**
     * Get the current detection threshold
     * @return Current threshold value
     */
    uint16_t getThreshold() const;

    /**
     * Turn the RED LED on
     */
    void ledOn();

    /**
     * Turn the RED LED off
     */
    void ledOff();

    /**
     * Set the RED LED state
     * @param state true = on, false = off
     */
    void setLed(bool state);

    /**
     * Get the current LED state
     * @return true if LED is on, false if off
     */
    bool isLedOn() const;

private:
    uint8_t _pin;
    uint8_t _ledPin;
    uint16_t _threshold;
    uint16_t _rawValue;
    bool _connected;
    bool _ledState;
};

#endif // SPO2_SENSOR_H
