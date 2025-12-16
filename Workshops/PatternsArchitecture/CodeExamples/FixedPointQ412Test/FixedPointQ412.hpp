#ifndef FIXED_POINT_Q412_HPP
#define FIXED_POINT_Q412_HPP

#include <cstdint>

namespace fixedpoint {

/**
 * @brief Q4.12 Fixed-point converter
 *
 * Converts between floating-point voltage and Q4.12 fixed-point format.
 * Q4.12 = 4 bits integer (0-15), 12 bits fraction (0-4095)
 * Suitable for STM32L432 12-bit ADC/DAC.
 *
 * This class was extracted from CSKompaktData to enable unit testing.
 * Previously the conversion functions were private static members,
 * making them impossible to test externally.
 *
 * Design Pattern: Humble Object Pattern (Clean Architecture Ch.23)
 * - Extracted testable logic from untestable context
 * - Enables off-target testing without hardware
 */
class FixedPointQ412 {
public:
    using FloatType = float;
    using FixedType = uint16_t;

    static constexpr uint16_t FRACTION_BITS = 12U;
    static constexpr uint16_t FRACTION_MASK = 0x0FFFU;
    static constexpr uint8_t  INTEGER_MASK  = 0x0FU;

    /**
     * @brief Convert floating-point to Q4.12 fixed-point
     * @param value Floating-point value in range [0.0, 15.999...]
     * @return Q4.12 fixed-point representation
     */
    static FixedType toFixed(FloatType value) {
        const auto integer = static_cast<uint8_t>(value) & INTEGER_MASK;
        const auto fraction = static_cast<uint16_t>((value - integer) * FRACTION_MASK);
        return static_cast<FixedType>((integer << FRACTION_BITS) + fraction);
    }

    /**
     * @brief Convert Q4.12 fixed-point to floating-point
     * @param fixed Q4.12 fixed-point value
     * @return Floating-point representation
     */
    static FloatType toFloat(FixedType fixed) {
        const uint16_t integer = fixed >> FRACTION_BITS;
        const uint16_t fraction = fixed & FRACTION_MASK;
        return static_cast<FloatType>(integer) +
               static_cast<FloatType>(fraction) / FRACTION_MASK;
    }

    /**
     * @brief Maximum quantization error for this format
     * @return Approximately 1/4095 = 0.000244
     */
    static constexpr FloatType maxError() {
        return 1.0F / FRACTION_MASK;
    }
};

}  // namespace fixedpoint

#endif  // FIXED_POINT_Q412_HPP
