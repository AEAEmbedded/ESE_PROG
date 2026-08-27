#include "temperature_controller.hpp"

namespace temperature {

TemperatureController::TemperatureController(ITemperatureSensor& sensor,
                                             IHeater& heater,
                                             const ControllerConfig& config)
    : m_sensor{sensor}
    , m_heater{heater}
    , m_config{config}
    , m_lastReading{0.0F}
    , m_inFault{false}
{
}

void TemperatureController::update() {
    if (!m_sensor.isHealthy()) {
        m_inFault = true;
        m_heater.turnOff();
        return;
    }

    m_inFault = false;
    m_lastReading = m_sensor.read();

    const float error = m_config.setpoint - m_lastReading;

    if (error > m_config.hysteresis) {
        m_heater.turnOn();
    } else if (error < -m_config.hysteresis) {
        m_heater.turnOff();
    }
    // Within hysteresis band: maintain current state
}

float TemperatureController::getSetpoint() const {
    return m_config.setpoint;
}

void TemperatureController::setSetpoint(float setpoint) {
    m_config.setpoint = setpoint;
}

bool TemperatureController::isInFault() const {
    return m_inFault;
}

float TemperatureController::getLastReading() const {
    return m_lastReading;
}

}  // namespace temperature