#pragma once

// Where measurements go. The Sampler and main() depend on this abstraction;
// MQTT is a detail behind it (DIP).

#include <string>

namespace station {

class Publisher {
public:
    virtual ~Publisher() = default;

    /// Send `payload` to `topic`. Returns false if it could not be sent now
    /// (broker down, not connected). Must not block for long: it is called
    /// from the sampler thread.
    virtual bool publish(const std::string& topic, const std::string& payload) = 0;
};

} // namespace station
