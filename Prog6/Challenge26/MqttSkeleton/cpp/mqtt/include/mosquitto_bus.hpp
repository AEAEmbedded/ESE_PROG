/**
 * @file mosquitto_bus.hpp
 * @brief IMessageBus over MQTT with libmosquitto.
 *
 * The only file in the skeleton that knows libmosquitto. Runs the network
 * loop on a background thread (mosquitto_loop_start), so handlers are called
 * from that thread: protect shared state in your classes.
 */

#ifndef SIM_MOSQUITTO_BUS_HPP
#define SIM_MOSQUITTO_BUS_HPP

#include <mutex>
#include <optional>
#include <string>
#include <vector>

#include "message_bus.hpp"

struct mosquitto;
struct mosquitto_message;

namespace sim {

class MosquittoBus : public IMessageBus {
public:
    /// Message the broker publishes (retained) if this client drops off.
    struct LastWill {
        std::string topic;
        std::string payload;
    };

    MosquittoBus(std::string clientId, std::string host, int port = 1883,
                 std::optional<LastWill> will = std::nullopt);
    ~MosquittoBus() override;

    MosquittoBus(const MosquittoBus&) = delete;
    MosquittoBus& operator=(const MosquittoBus&) = delete;

    /// Connects and starts the network thread. Throws std::runtime_error.
    void connect();
    void disconnect();

    void publish(const std::string& topic, const std::string& payload, bool retain = false) override;
    void subscribe(const std::string& filter, Handler handler) override;

private:
    struct Subscription {
        std::string filter;
        Handler handler;
    };

    static void onConnect(mosquitto* client, void* self, int reasonCode);
    static void onMessage(mosquitto* client, void* self, const mosquitto_message* message);

    std::string host_;
    int port_;
    mosquitto* client_ = nullptr;
    std::mutex mutex_;
    std::vector<Subscription> subscriptions_;
};

} // namespace sim

#endif // SIM_MOSQUITTO_BUS_HPP
