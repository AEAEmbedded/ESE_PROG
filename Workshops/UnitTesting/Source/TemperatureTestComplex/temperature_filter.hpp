#ifndef TEMPERATURE_FILTER_HPP
#define TEMPERATURE_FILTER_HPP

#include <cstdint>
#include <array>

namespace temperature {

/// @brief Moving average filter for temperature readings
/// @details Smooths noisy ADC readings over a configurable window
template<uint8_t WINDOW_SIZE = 4U>
class TemperatureFilter {
public:
    TemperatureFilter();

    /// @brief Add a new reading to the filter
    /// @param reading Temperature reading in Celsius
    void addReading(float reading);

    /// @brief Get the filtered (averaged) temperature
    /// @return Average of readings in the window
    float getFiltered() const;

    /// @brief Check if filter has enough samples
    /// @return true if window is fully populated
    bool isReady() const;

    /// @brief Reset filter to initial state
    void reset();

    /// @brief Get number of samples currently in filter
    /// @return Sample count (0 to WINDOW_SIZE)
    uint8_t getSampleCount() const;

private:
    std::array<float, WINDOW_SIZE> m_readings;
    uint8_t m_index;
    uint8_t m_count;
};

// ============================================================================
// Template Implementation
// ============================================================================

template<uint8_t WINDOW_SIZE>
TemperatureFilter<WINDOW_SIZE>::TemperatureFilter()
    : m_readings{}
    , m_index{0U}
    , m_count{0U}
{
}

template<uint8_t WINDOW_SIZE>
void TemperatureFilter<WINDOW_SIZE>::addReading(float reading) {
    m_readings[m_index] = reading;
    m_index = static_cast<uint8_t>((m_index + 1U) % WINDOW_SIZE);
    
    if (m_count < WINDOW_SIZE) {
        ++m_count;
    }
}

template<uint8_t WINDOW_SIZE>
float TemperatureFilter<WINDOW_SIZE>::getFiltered() const {
    if (m_count == 0U) {
        return 0.0F;
    }
    
    float sum = 0.0F;
    for (uint8_t i = 0U; i < m_count; ++i) {
        sum += m_readings[i];
    }
    
    return sum / static_cast<float>(m_count);
}

template<uint8_t WINDOW_SIZE>
bool TemperatureFilter<WINDOW_SIZE>::isReady() const {
    return m_count == WINDOW_SIZE;
}

template<uint8_t WINDOW_SIZE>
void TemperatureFilter<WINDOW_SIZE>::reset() {
    m_index = 0U;
    m_count = 0U;
}

template<uint8_t WINDOW_SIZE>
uint8_t TemperatureFilter<WINDOW_SIZE>::getSampleCount() const {
    return m_count;
}

}  // namespace temperature

#endif  // TEMPERATURE_FILTER_HPP