/**
 * @file virtual_thermometer.hpp
 * @brief A virtual thermometer SimDevice.
 *
 * The "connection: virtual" variant of the challenge document. For the
 * engine it is indistinguishable from the hacked donor thermometer, because
 * both use the same topics:
 *
 *   subscribes  scenario/devices/<ID>/command/{body,surface}   {"value":37.2,"unit":"C",...}
 *   publishes   scenario/devices/<ID>/event/measured           {"value":37.2,"unit":"C","mode":"body",...}
 *   publishes   scenario/devices/<ID>/status (retained)        {"connected":true,"battery":100,...}
 *
 * The class depends only on IMessageBus and a clock, so it is fully unit
 * tested with a FakeBus (tests/test_virtual_thermometer.cpp). The same shape
 * works for your firmware: a Thermometer class that combines a display, a
 * buzzer, a trigger and a connection, each behind an interface.
 */

#ifndef SIM_VIRTUAL_THERMOMETER_HPP
#define SIM_VIRTUAL_THERMOMETER_HPP

#include <cstdint>
#include <functional>
#include <map>
#include <mutex>
#include <optional>
#include <string>

#include "message_bus.hpp"

namespace sim {

class VirtualThermometer {
public:
    enum class Mode { Body, Surface };
    using Clock = std::function<int64_t()>;

    VirtualThermometer(IMessageBus& bus, std::string deviceId, Clock clock);

    /// Subscribes to its commands and announces itself as connected.
    void start();
    /// Announces itself as disconnected.
    void stop();

    void setMode(Mode mode);
    Mode mode() const;

    /// The temperature the engine last sent for the current mode.
    std::optional<double> reading() const;

    /// The nurse pulls the trigger: publishes a measured event. Returns false
    /// when the engine has not sent a value for the current mode yet.
    bool pullTrigger();

    static std::string toString(Mode mode);
    static std::optional<Mode> parseMode(const std::string& text);

private:
    void onCommand(const std::string& topic, const std::string& payload);

    IMessageBus& bus_;
    std::string deviceId_;
    Clock clock_;
    mutable std::mutex mutex_;
    Mode mode_ = Mode::Body;
    std::map<Mode, double> values_;
    int battery_ = 100;
};

} // namespace sim

#endif // SIM_VIRTUAL_THERMOMETER_HPP
