#ifndef CS_KOMPAKT_DATA_HPP
#define CS_KOMPAKT_DATA_HPP

#include "FixedPointQ412.hpp"
#include <cstdint>

namespace kompaktdata {

// Type aliases matching original code style
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
