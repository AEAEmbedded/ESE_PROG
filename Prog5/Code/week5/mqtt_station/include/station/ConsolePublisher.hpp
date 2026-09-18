#pragma once

#include "station/Publisher.hpp"

#include <cstdio>

namespace station {

/// Prints instead of publishing. Handy on a laptop and in tests.
class ConsolePublisher final : public Publisher {
public:
    bool publish(const std::string& topic, const std::string& payload) override
    {
        std::printf("%s %s\n", topic.c_str(), payload.c_str());
        std::fflush(stdout);
        return true;
    }
};

} // namespace station
