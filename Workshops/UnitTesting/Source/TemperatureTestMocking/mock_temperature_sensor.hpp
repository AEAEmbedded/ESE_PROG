#ifndef MOCK_TEMPERATURE_SENSOR_HPP
#define MOCK_TEMPERATURE_SENSOR_HPP

#include "i_temperature_sensor.hpp"

namespace temperature {
namespace test {

/// @brief Manual mock for testing
class MockTemperatureSensor : public ITemperatureSensor {
public:
    MockTemperatureSensor()
        : m_temperature{20.0F}
        , m_healthy{true}
        , m_readCount{0U}
    {
    }

    float read() override {
        ++m_readCount;
        return m_temperature;
    }

    bool isHealthy() const override {
        return m_healthy;
    }

    // Test control methods
    void setTemperature(float temperature) {
        m_temperature = temperature;
    }

    void setHealthy(bool healthy) {
        m_healthy = healthy;
    }

    uint32_t getReadCount() const {
        return m_readCount;
    }

    void resetReadCount() {
        m_readCount = 0U;
    }

private:
    float m_temperature;
    bool m_healthy;
    uint32_t m_readCount;
};

}  // namespace test
}  // namespace temperature

#endif  // MOCK_TEMPERATURE_SENSOR_HPP