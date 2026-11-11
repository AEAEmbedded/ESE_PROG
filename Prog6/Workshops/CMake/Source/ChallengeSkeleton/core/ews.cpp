#include "ews.hpp"

#include <algorithm>

namespace ews {

EarlyWarningScore::EarlyWarningScore(std::vector<ScoreBand> heartRateBands)
    : heartRateBands_(std::move(heartRateBands)), worstScore_(0U) {
    for (const ScoreBand& band : heartRateBands_) {
        worstScore_ = std::max(worstScore_, band.score);
    }
}

uint8_t EarlyWarningScore::scoreHeartRate(float beatsPerMinute) const {
    for (const ScoreBand& band : heartRateBands_) {
        if ((beatsPerMinute >= band.lower) && (beatsPerMinute <= band.upper)) {
            return band.score;
        }
    }
    return worstScore_;
}

} // namespace ews
