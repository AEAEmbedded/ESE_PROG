#include <iostream>
#include <vector>

#include "ews.hpp"

int main() {
    // Example band table for ONE age band. Values are illustrative, not
    // Dutch PEWS. In the real engine this table comes from configuration.
    const std::vector<ews::ScoreBand> infantHeartRateBands = {
        {110.0F, 150.0F, 0U},
        {90.0F, 109.0F, 1U},
        {151.0F, 170.0F, 1U},
        {70.0F, 89.0F, 2U},
        {171.0F, 190.0F, 2U},
    };

    const ews::EarlyWarningScore score(infantHeartRateBands);

    for (const float bpm : {120.0F, 95.0F, 180.0F, 40.0F}) {
        std::cout << "Heart rate " << bpm << " bpm -> score "
                  << static_cast<int>(score.scoreHeartRate(bpm)) << '\n';
    }

    return 0;
}
