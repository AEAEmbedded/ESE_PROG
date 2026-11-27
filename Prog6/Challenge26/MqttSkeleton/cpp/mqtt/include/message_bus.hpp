/**
 * @file message_bus.hpp
 * @brief The connection to the rest of the system, as an interface.
 *
 * A SimDevice, the engine or an instructor view only depends on this
 * interface. MosquittoBus implements it over MQTT; the tests implement it
 * with a FakeBus in memory. This is the "connection behind an interface"
 * that the challenge asks for: swapping Wi-Fi, BLE-via-gateway or virtual
 * must not change the device class.
 */

#ifndef SIM_MESSAGE_BUS_HPP
#define SIM_MESSAGE_BUS_HPP

#include <functional>
#include <string>

namespace sim {

class IMessageBus {
public:
    /// Called with the concrete topic and the raw payload. Note that a real
    /// bus calls handlers from its network thread.
    using Handler = std::function<void(const std::string& topic, const std::string& payload)>;

    virtual ~IMessageBus() = default;

    virtual void publish(const std::string& topic, const std::string& payload, bool retain = false) = 0;

    /// Route every message matching `filter` (may contain + and #) to `handler`.
    virtual void subscribe(const std::string& filter, Handler handler) = 0;
};

} // namespace sim

#endif // SIM_MESSAGE_BUS_HPP
