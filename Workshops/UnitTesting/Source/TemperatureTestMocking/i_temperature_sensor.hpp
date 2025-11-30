#ifndef I_TEMPERATURE_SENSOR_HPP
#define I_TEMPERATURE_SENSOR_HPP

namespace temperature {

/// @brief Interface for temperature sensors
/// @details Allows dependency injection for testability
class ITemperatureSensor {
public:
    virtual ~ITemperatureSensor() = default;

    /// @brief Read current temperature
    /// @return Temperature in degrees Celsius
    virtual float read() = 0;

    /// @brief Check if sensor is functioning
    /// @return true if sensor is healthy
    virtual bool isHealthy() const = 0;
};

}  // namespace temperature

#endif  // I_TEMPERATURE_SENSOR_HPP