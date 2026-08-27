#include "CppUTest/TestHarness.h"
#include "CSKompaktData.hpp"

using namespace kompaktdata;

// ============================================================================
// CSKompaktData Integration Tests
// ============================================================================

TEST_GROUP(CSKompaktData) {
};

TEST(CSKompaktData, DefaultConstructorInitializesToZero) {
    CSKompaktData data;

    DOUBLES_EQUAL(0.0F, data.geefMeting(), 0.001);
    DOUBLES_EQUAL(0.0F, data.geefReferentie(), 0.001);
    DOUBLES_EQUAL(0.0F, data.geefSetpoint(), 0.001);
}

TEST(CSKompaktData, ConstructorStoresValuesCorrectly) {
    constexpr SampleMoment SAMPLE = 42;
    constexpr Spanning METING = 3.3F;
    constexpr Spanning REFERENTIE = 2.5F;
    constexpr Spanning SETPOINT = 1.8F;

    CSKompaktData data(SAMPLE, METING, REFERENTIE, SETPOINT);

    LONGS_EQUAL(SAMPLE, data.n);
    DOUBLES_EQUAL(METING, data.geefMeting(), CSKompaktData::Converter::maxError());
    DOUBLES_EQUAL(REFERENTIE, data.geefReferentie(), CSKompaktData::Converter::maxError());
    DOUBLES_EQUAL(SETPOINT, data.geefSetpoint(), CSKompaktData::Converter::maxError());
}

TEST(CSKompaktData, RawAccessReturnsFixedPointValues) {
    constexpr Spanning VOLTAGE = 5.0F;

    CSKompaktData data(0, VOLTAGE, VOLTAGE, VOLTAGE);

    // 5.0 in Q4.12 = 0x5000
    LONGS_EQUAL(0x5000, data.geefMetingRaw());
}

TEST(CSKompaktData, TypicalPidControllerValues) {
    // Simulate typical PID controller scenario
    constexpr Spanning MEASURED = 3.28F;   // Measured voltage
    constexpr Spanning REFERENCE = 3.30F;  // Reference voltage
    constexpr Spanning CONTROL = 0.15F;    // Control output

    CSKompaktData data(100, MEASURED, REFERENCE, CONTROL);

    // Values should round-trip within quantization error
    DOUBLES_EQUAL(MEASURED, data.geefMeting(), CSKompaktData::Converter::maxError());
    DOUBLES_EQUAL(REFERENCE, data.geefReferentie(), CSKompaktData::Converter::maxError());
    DOUBLES_EQUAL(CONTROL, data.geefSetpoint(), CSKompaktData::Converter::maxError());
}

// ============================================================================
// Workshop Discussion Point
// ============================================================================

// NOTE: In the ORIGINAL code, there was a doeZelftest() method:
//
// FoutCode CSKompaktData::doeZelftest() {
//     static constexpr Spanning uTest = 3.45678f;
//     const auto fpTest = konverteerSpanning(uTest);
//     const auto uUitkomst = konverteerFixedPoint(fpTest);
//     // ...
// }
//
// This existed because the author KNEW testing was needed but
// couldn't access the private static methods from outside.
//
// NOW: We test FixedPointQ412 directly in test_fixed_point_q412.cpp
// The self-test hack is no longer necessary!
//
// This is the Humble Object Pattern (Clean Architecture Ch.23):
// - Extract testable logic (FixedPointQ412)
// - Leave simple delegation in the "humble" object (CSKompaktData)
