#ifndef TEMPERATURE_CONTROLLER_HPP
#define TEMPERATURE_CONTROLLER_HPP

#include "i_temperature_sensor.hpp"
#include "i_heater.hpp"

namespace temperature {

/// @brief Controller configuration
struct ControllerConfig {
    float setpoint = 20.0F;       ///< Target temperature in Celsius
    float hysteresis = 1.0F;      ///< Dead band to prevent oscillation
};

/// @brief Simple on/off temperature controller
/// @details Uses hysteresis to prevent rapid switching
class TemperatureController {
public:
    /// @brief Construct controller with injected dependencies
    /// @param sensor Temperature sensor (caller owns lifetime)
    /// @param heater Heater output (caller owns lifetime)
    /// @param config Controller configuration
    TemperatureController(ITemperatureSensor& sensor, 
                          IHeater& heater,
                          const ControllerConfig& config = ControllerConfig{});

    /// @brief Run one control cycle
    /// @details Reads sensor, decides heater state
    void update();

    /// @brief Get current setpoint
    /// @return Target temperature in Celsius
    float getSetpoint() const;

    /// @brief Change setpoint
    /// @param setpoint New target temperature in Celsius
    void setSetpoint(float setpoint);

    /// @brief Check if controller is in fault state
    /// @return true if sensor is unhealthy
    bool isInFault() const;

    /// @brief Get last temperature reading
    /// @return Last read temperature in Celsius
    float getLastReading() const;

private:
    ITemperatureSensor& m_sensor;
    IHeater& m_heater;
    ControllerConfig m_config;
    float m_lastReading;
    bool m_inFault;
};

}  // namespace temperature

#endif  // TEMPERATURE_CONTROLLER_HPP