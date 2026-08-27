#include "CppUTest/TestHarness.h"
#include "FixedPointQ412.hpp"

using namespace fixedpoint;

// ============================================================================
// Round-trip Conversion Tests
// ============================================================================

TEST_GROUP(RoundTrip) {
};

TEST(RoundTrip, ValuePreservedWithinQuantizationError) {
    constexpr float INPUT = 3.45678F;

    auto fixed = FixedPointQ412::toFixed(INPUT);
    auto result = FixedPointQ412::toFloat(fixed);

    DOUBLES_EQUAL(INPUT, result, FixedPointQ412::maxError());
}

TEST(RoundTrip, ZeroConvertsExactly) {
    constexpr float INPUT = 0.0F;

    auto fixed = FixedPointQ412::toFixed(INPUT);
    auto result = FixedPointQ412::toFloat(fixed);

    DOUBLES_EQUAL(INPUT, result, 0.0001);
}

TEST(RoundTrip, IntegerValueConvertsExactly) {
    constexpr float INPUT = 5.0F;

    auto fixed = FixedPointQ412::toFixed(INPUT);
    auto result = FixedPointQ412::toFloat(fixed);

    DOUBLES_EQUAL(INPUT, result, 0.0001);
}

// ============================================================================
// toFixed() Tests
// ============================================================================

TEST_GROUP(ToFixed) {
};

TEST(ToFixed, ZeroProducesZeroFixed) {
    LONGS_EQUAL(0x0000, FixedPointQ412::toFixed(0.0F));
}

TEST(ToFixed, MaxIntegerProducesCorrectFixed) {
    // Q4.12: integer 15 = 0xF, shifted left 12 bits = 0xF000
    LONGS_EQUAL(0xF000, FixedPointQ412::toFixed(15.0F));
}

TEST(ToFixed, OneProducesCorrectFixed) {
    // Q4.12: integer 1 = 0x1, shifted left 12 bits = 0x1000
    LONGS_EQUAL(0x1000, FixedPointQ412::toFixed(1.0F));
}

// TODO: Students implement
// TEST(ToFixed, HalfProducesCorrectFixed)
// Hint: 0.5 in Q4.12 should be approximately 0x0800 (2048/4095)

// TODO: Students implement
// TEST(ToFixed, QuarterProducesCorrectFixed)
// Hint: 0.25 in Q4.12 should be approximately 0x0400 (1024/4095)

// ============================================================================
// toFloat() Tests
// ============================================================================

TEST_GROUP(ToFloat) {
};

TEST(ToFloat, ZeroFixedProducesZero) {
    DOUBLES_EQUAL(0.0F, FixedPointQ412::toFloat(0x0000), 0.0001);
}

TEST(ToFloat, MaxIntegerFixedProducesMaxInteger) {
    DOUBLES_EQUAL(15.0F, FixedPointQ412::toFloat(0xF000), 0.0001);
}

TEST(ToFloat, IntegerPlusFractionConvertsCorrectly) {
    // 5.25 -> integer=5 (0x5000), fraction=0.25 (~0x0400)
    // Expected fixed: 0x5400 (approx)
    constexpr float EXPECTED = 5.25F;
    constexpr uint16_t FIXED_5_25 = 0x53FF;  // 5 + 1023/4095

    auto result = FixedPointQ412::toFloat(FIXED_5_25);

    DOUBLES_EQUAL(EXPECTED, result, FixedPointQ412::maxError());
}

// TODO: Students implement
// TEST(ToFloat, MidpointValueConvertsCorrectly)
// Hint: 0x8000 should produce approximately 8.0

// ============================================================================
// Edge Cases
// ============================================================================

TEST_GROUP(EdgeCases) {
};

TEST(EdgeCases, MaxValueConvertsCorrectly) {
    // 0xFFFF = integer 15, fraction 4095/4095 = 15.999...
    constexpr uint16_t MAX_FIXED = 0xFFFF;
    constexpr float EXPECTED_MAX = 15.0F + (4095.0F / 4095.0F);

    DOUBLES_EQUAL(EXPECTED_MAX, FixedPointQ412::toFloat(MAX_FIXED), 0.001);
}

TEST(EdgeCases, VoltageTypicalAdcRange) {
    // Typical STM32 ADC: 0-3.3V mapped to 12-bit (0-4095)
    // If we store 3.3V in Q4.12 format:
    constexpr float VOLTAGE = 3.3F;

    auto fixed = FixedPointQ412::toFixed(VOLTAGE);
    auto result = FixedPointQ412::toFloat(fixed);

    DOUBLES_EQUAL(VOLTAGE, result, FixedPointQ412::maxError());
}

// TODO: Students implement
// TEST(EdgeCases, NegativeValueBehavior)
// Question: What happens with negative input? Is this a valid use case?
// Hint: The current implementation uses unsigned types...
