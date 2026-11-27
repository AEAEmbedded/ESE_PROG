/**
 * @file topics.hpp
 * @brief The MQTT topic set of the challenge, as constants and builders.
 *
 * Single source of truth for topic names in the C++ skeleton. It mirrors the
 * table "Minimal MQTT topic set" of the challenge document and
 * `python/topics.py`. Extend it if you need to, but keep the existing names
 * and payloads intact so components of different teams keep working together.
 *
 * Header-only and free of I/O, so it is usable from firmware-like code and
 * from unit tests alike.
 */

#ifndef SIM_TOPICS_HPP
#define SIM_TOPICS_HPP

#include <string>
#include <string_view>

namespace sim::topics {

// --- Fixed topics ---------------------------------------------------------

inline constexpr std::string_view kStateCurrent = "scenario/state/current";
inline constexpr std::string_view kStateVitals = "scenario/state/vitals";
inline constexpr std::string_view kInterventionsPerformed = "scenario/interventions/performed";
inline constexpr std::string_view kInterventionsFeedback = "scenario/interventions/feedback";
inline constexpr std::string_view kDebrief = "scenario/debrief";

// Wildcards for subscribers
inline constexpr std::string_view kControlAll = "scenario/control/+";
inline constexpr std::string_view kDevicesAll = "scenario/devices/#";
inline constexpr std::string_view kDeviceStatusAll = "scenario/devices/+/status";
inline constexpr std::string_view kDeviceEventsAll = "scenario/devices/+/event/+";
inline constexpr std::string_view kEverything = "scenario/#";

// --- Topic builders -------------------------------------------------------

/// scenario/control/<command>, command in {load, start, pause, stop}.
inline std::string control(const std::string& command) {
    return "scenario/control/" + command;
}

/// scenario/devices/<ID>/command/<param>, e.g. the body temperature to show.
inline std::string deviceCommand(const std::string& deviceId, const std::string& param) {
    return "scenario/devices/" + deviceId + "/command/" + param;
}

/// All commands for one device.
inline std::string deviceCommandFilter(const std::string& deviceId) {
    return "scenario/devices/" + deviceId + "/command/+";
}

/// scenario/devices/<ID>/event/<event>, e.g. "measured", "shock".
inline std::string deviceEvent(const std::string& deviceId, const std::string& event) {
    return "scenario/devices/" + deviceId + "/event/" + event;
}

/// scenario/devices/<ID>/status, published retained with a last will.
inline std::string deviceStatus(const std::string& deviceId) {
    return "scenario/devices/" + deviceId + "/status";
}

/// The last segment of a topic: the <param> of a command, the <event> of an
/// event, the <command> of a control topic.
inline std::string lastSegment(std::string_view topic) {
    const auto slash = topic.rfind('/');
    return std::string(slash == std::string_view::npos ? topic : topic.substr(slash + 1));
}

/// MQTT topic filter matching with '+' (one level) and '#' (rest). Used by
/// the real bus and by the FakeBus in the tests, so both behave the same.
inline bool matches(std::string_view filter, std::string_view topic) {
    while (!filter.empty() || !topic.empty()) {
        const auto fSlash = filter.find('/');
        const auto tSlash = topic.find('/');
        const auto fPart = filter.substr(0, fSlash);
        const auto tPart = topic.substr(0, tSlash);

        if (fPart == "#") {
            return true;
        }
        if (fPart != "+" && fPart != tPart) {
            return false;
        }
        if (fSlash == std::string_view::npos || tSlash == std::string_view::npos) {
            return fSlash == tSlash;  // both at their last level
        }
        filter.remove_prefix(fSlash + 1);
        topic.remove_prefix(tSlash + 1);
    }
    return true;
}

} // namespace sim::topics

#endif // SIM_TOPICS_HPP
