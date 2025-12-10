/**
 * @file Exercise_4.cpp
 * @brief Demonstration of clean commenting principles
 *
 * This file contrasts undocumented code with properly documented code.
 * Remember: code is read more than 10x more often than it is written.
 */

/**
 * @brief Reads atmospheric pressure from the BMP280 sensor.
 *
 * Performs a pressure measurement with configurable oversampling for
 * noise reduction. Higher oversampling improves accuracy but increases
 * measurement time.
 *
 * @param[in] oversample  Oversampling factor (0-5):
 *                        - 0: Skipped (no measurement)
 *                        - 1: 1x  (16-bit, 2.62 Pa RMS noise)
 *                        - 2: 2x  (17-bit, 1.31 Pa RMS noise)
 *                        - 3: 4x  (18-bit, 0.66 Pa RMS noise)
 *                        - 4: 8x  (19-bit, 0.33 Pa RMS noise)
 *                        - 5: 16x (20-bit, 0.16 Pa RMS noise)
 *
 * @param[in] blocking    Measurement wait behavior:
 *                        - true:  Busy-waits until conversion completes
 *                                 (blocks CPU, ~2-44ms depending on oversample)
 *                        - false: Returns immediately if conversion in progress,
 *                                 caller must poll isConversionComplete()
 *
 * @return Pressure in Pa (Pascals) as Q24.8 fixed-point.
 *         Divide by 256 to get integer Pa, or by 25600 for hPa/mbar.
 *         Returns INT32_MIN (-2147483648) on error:
 *         - Sensor not initialized (call begin() first)
 *         - I2C communication failure
 *         - Invalid oversample value
 *         - Non-blocking call while conversion in progress
 *
 * @pre    begin() must have been called successfully.
 * @pre    I2C bus must be initialized and available.
 *
 * @note   For altitude calculations, also read temperature as pressure
 *         compensation requires the current temperature value.
 *
 * @warning Do not call from ISR context when blocking=true.
 *
 * @see    begin(), readTemperature(), isConversionComplete()
 *
 * @code
 *   // Example: Read pressure in hPa (mbar)
 *   int32_t raw = readPressure(3, true);  // 4x oversample, blocking
 *   if (raw != INT32_MIN) {
 *       float pressure_hPa = raw / 25600.0f;
 *       printf("Pressure: %.2f hPa\n", pressure_hPa);
 *   }
 * @endcode
 */
int32_t readPressure_documented(uint8_t oversample, bool blocking);
