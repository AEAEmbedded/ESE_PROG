#ifndef MOCK_HEATER_HPP
#define MOCK_HEATER_HPP

#include "i_heater.hpp"
#include <cstdint>

namespace temperature {
namespace test {

/// @brief Manual mock for testing
class MockHeater : public IHeater {
public:
    MockHeater()
        : m_isOn{false}
        , m_turnOnCount{0U}
        , m_turnOffCount{0U}
    {
    }

    void turnOn() override {
        m_isOn = true;
        ++m_turnOnCount;
    }

    void turnOff() override {
        m_isOn = false;
        ++m_turnOffCount;
    }

    bool isOn() const override {
        return m_isOn;
    }

    // Test inspection methods
    uint32_t getTurnOnCount() const {
        return m_turnOnCount;
    }

    uint32_t getTurnOffCount() const {
        return m_turnOffCount;
    }

    void resetCounts() {
        m_turnOnCount = 0U;
        m_turnOffCount = 0U;
    }

private:
    bool m_isOn;
    uint32_t m_turnOnCount;
    uint32_t m_turnOffCount;
};

}  // namespace test
}  // namespace temperature

#endif  // MOCK_HEATER_HPP