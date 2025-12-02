/**
 * @brief Reads temperature from sensor
 * @param sensor Pointer to sensor instance
 * @return Temperature value in degrees Celsius
 */
float readTemperature(Sensor* sensor) {
    // Read the raw value from the sensor
    uint16_t raw = sensor->read();
    // Convert to temperature using formula
    return raw * 0.0625f - 40.0f;
}