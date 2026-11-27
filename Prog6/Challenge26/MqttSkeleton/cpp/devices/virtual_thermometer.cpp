#include "virtual_thermometer.hpp"

#include "payload.hpp"
#include "topics.hpp"

namespace sim {

VirtualThermometer::VirtualThermometer(IMessageBus& bus, std::string deviceId, Clock clock)
    : bus_(bus), deviceId_(std::move(deviceId)), clock_(std::move(clock)) {}

void VirtualThermometer::start() {
    bus_.subscribe(topics::deviceCommandFilter(deviceId_),
                   [this](const std::string& topic, const std::string& payload) { onCommand(topic, payload); });
    bus_.publish(topics::deviceStatus(deviceId_),
                 Payload().add("timestamp", clock_()).add("connected", true)
                          .add("battery", battery_).add("type", "thermometer").str(),
                 /*retain=*/true);
}

void VirtualThermometer::stop() {
    bus_.publish(topics::deviceStatus(deviceId_),
                 Payload().add("timestamp", clock_()).add("connected", false).add("reason", "shutdown").str(),
                 /*retain=*/true);
}

void VirtualThermometer::setMode(Mode mode) {
    const std::lock_guard<std::mutex> lock(mutex_);
    mode_ = mode;
}

VirtualThermometer::Mode VirtualThermometer::mode() const {
    const std::lock_guard<std::mutex> lock(mutex_);
    return mode_;
}

std::optional<double> VirtualThermometer::reading() const {
    const std::lock_guard<std::mutex> lock(mutex_);
    const auto it = values_.find(mode_);
    return it == values_.end() ? std::nullopt : std::optional<double>(it->second);
}

bool VirtualThermometer::pullTrigger() {
    Mode mode;
    std::optional<double> value;
    {
        const std::lock_guard<std::mutex> lock(mutex_);
        mode = mode_;
        const auto it = values_.find(mode_);
        if (it != values_.end()) {
            value = it->second;
        }
    }
    if (!value) {
        return false;
    }
    bus_.publish(topics::deviceEvent(deviceId_, "measured"),
                 Payload().add("timestamp", clock_()).add("value", *value)
                          .add("unit", "C").add("mode", toString(mode)).str());
    return true;
}

void VirtualThermometer::onCommand(const std::string& topic, const std::string& payload) {
    const auto mode = parseMode(topics::lastSegment(topic));
    const auto value = extractNumber(payload, "value");
    if (!mode || !value) {
        return;  // unknown parameter or malformed payload: ignore, like a real device would
    }
    const std::lock_guard<std::mutex> lock(mutex_);
    values_[*mode] = *value;
}

std::string VirtualThermometer::toString(Mode mode) {
    return mode == Mode::Body ? "body" : "surface";
}

std::optional<VirtualThermometer::Mode> VirtualThermometer::parseMode(const std::string& text) {
    if (text == "body") {
        return Mode::Body;
    }
    if (text == "surface") {
        return Mode::Surface;
    }
    return std::nullopt;
}

} // namespace sim
