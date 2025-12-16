#ifndef CS_KOMPAKT_DATA_HPP
#define CS_KOMPAKT_DATA_HPP

/**
 * =============================================================================
 * DISCLAIMER
 * =============================================================================
 * This code is intentionally left "unprofessional" for educational purposes.
 *
 * We only refactored the ARCHITECTURE (extracting FixedPointQ412), not the
 * code quality. The original code was written by a colleague with limited
 * software engineering experience, and contains issues such as:
 *
 * - Dutch naming conventions (Spanning, geefMeting, geefReferentie, etc.)
 * - Inconsistent style
 * - Poor naming choices
 *
 * In a real refactoring effort, you would also:
 * - Rename to English (Spanning -> Voltage, geefMeting -> getMeasurement)
 * - Apply consistent naming conventions (camelCase or snake_case)
 * - Add proper documentation
 * - Review for MISRA/coding standard compliance
 *
 * The point of this workshop is to demonstrate that ARCHITECTURAL improvements
 * (testability, separation of concerns) are separate from CODE QUALITY
 * improvements. Both matter, but they are different concerns.
 * =============================================================================
 */

#include "FixedPointQ412.hpp"
#include <cstdint>

namespace kompaktdata {

// Type aliases matching original code style (Dutch naming intentionally preserved)
using Spanning = float;
using UInt16 = uint16_t;
using SampleMoment = uint16_t;

/**
 * @brief Simplified CSKompaktData - AFTER refactoring
 *
 * Key changes from original:
 * 1. Uses extracted FixedPointQ412 converter (no private static methods)
 * 2. doeZelftest() is NO LONGER NEEDED - we test the converter directly
 * 3. Conversion logic is now reusable and testable
 *
 * Compare with original (BEFORE):
 * - konverteerSpanning() was private static - untestable
 * - konverteerFixedPoint() was private static - untestable
 * - doeZelftest() existed because author couldn't test externally
 */
class CSKompaktData {
public:
    using Converter = fixedpoint::FixedPointQ412;

    CSKompaktData() = default;

    explicit CSKompaktData(const SampleMoment nm, const Spanning mv,
                           const Spanning rv, const Spanning cv)
        : n(nm)
        , measurementValue(Converter::toFixed(mv))
        , referenceValue(Converter::toFixed(rv))
        , controlValue(Converter::toFixed(cv))
    {}

    // Getters use the extracted converter
    Spanning geefMeting() const {
        return Converter::toFloat(measurementValue);
    }

    Spanning geefReferentie() const {
        return Converter::toFloat(referenceValue);
    }

    Spanning geefSetpoint() const {
        return Converter::toFloat(controlValue);
    }

    // Raw access for serialization (if needed)
    UInt16 geefMetingRaw() const { return measurementValue; }
    UInt16 geefReferentieRaw() const { return referenceValue; }
    UInt16 geefSetpointRaw() const { return controlValue; }

    SampleMoment n{0};

private:
    UInt16 measurementValue{0};
    UInt16 referenceValue{0};
    UInt16 controlValue{0};

    // NOTE: doeZelftest() is REMOVED
    // The converter is now tested via unit tests in test_fixed_point_q412.cpp
    // This is the Humble Object Pattern in action!
};

}  // namespace kompaktdata

#endif  // CS_KOMPAKT_DATA_HPP
