// Project and standard headers must come BEFORE CppUTest/TestHarness.h:
// CppUTest replaces new/malloc with leak-detecting macros, which breaks
// standard library headers included after it.
#include <vector>

#include "ews.hpp"

#include <CppUTest/TestHarness.h>

namespace {
const std::vector<ews::ScoreBand> kBands = {
    {110.0F, 150.0F, 0U},
    {90.0F, 109.0F, 1U},
    {151.0F, 170.0F, 1U},
    {70.0F, 89.0F, 2U},
    {171.0F, 190.0F, 2U},
};
} // namespace

TEST_GROUP(EarlyWarningScore){};

TEST(EarlyWarningScore, NormalHeartRateScoresZero) {
    const ews::EarlyWarningScore score(kBands);
    CHECK_EQUAL(0, score.scoreHeartRate(120.0F));
}

TEST(EarlyWarningScore, MildlyAbnormalHeartRateScoresOne) {
    const ews::EarlyWarningScore score(kBands);
    CHECK_EQUAL(1, score.scoreHeartRate(95.0F));
    CHECK_EQUAL(1, score.scoreHeartRate(160.0F));
}

TEST(EarlyWarningScore, BandEdgesAreInclusive) {
    const ews::EarlyWarningScore score(kBands);
    CHECK_EQUAL(0, score.scoreHeartRate(110.0F));
    CHECK_EQUAL(0, score.scoreHeartRate(150.0F));
}

TEST(EarlyWarningScore, OutsideAllBandsScoresWorst) {
    const ews::EarlyWarningScore score(kBands);
    CHECK_EQUAL(2, score.scoreHeartRate(40.0F));
    CHECK_EQUAL(2, score.scoreHeartRate(250.0F));
}
