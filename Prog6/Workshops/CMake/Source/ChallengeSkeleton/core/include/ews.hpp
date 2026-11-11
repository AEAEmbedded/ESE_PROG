/**
 * @file ews.hpp
 * @brief Simplified early warning score, configured with a band table.
 *
 * This is deliberately NOT the real Dutch PEWS. It scores a single
 * parameter (heart rate) against a configurable band table. What it does
 * show is the structure the challenge asks for: thresholds come from
 * configuration, not from code (Open-Closed principle), so the same
 * scoring engine works for every age band.
 */

#ifndef EWS_HPP
#define EWS_HPP

#include <cstdint>
#include <vector>

namespace ews {

/// One scoring band: a value in [lower, upper] earns `score` points.
struct ScoreBand {
    float lower;
    float upper;
    uint8_t score;
};

class EarlyWarningScore {
public:
    /// The band table typically comes from a configuration file,
    /// one table per age band.
    explicit EarlyWarningScore(std::vector<ScoreBand> heartRateBands);

    /// Score a heart rate in beats per minute. Returns the score of the
    /// first matching band, or the worst (highest) band score when the
    /// value falls outside every band.
    uint8_t scoreHeartRate(float beatsPerMinute) const;

private:
    std::vector<ScoreBand> heartRateBands_;
    uint8_t worstScore_;
};

} // namespace ews

#endif // EWS_HPP
