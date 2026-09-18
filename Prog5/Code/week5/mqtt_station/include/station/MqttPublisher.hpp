#pragma once

// Publisher on top of libmosquitto. Connects in the constructor, runs the
// network loop on mosquitto's own thread, disconnects in the destructor (RAII).
//
//   sudo apt install libmosquitto-dev mosquitto mosquitto-clients   (Pi / Debian)
//   brew install mosquitto                                          (macOS)

#include "station/Publisher.hpp"

#include <atomic>
#include <string>

struct mosquitto;   // forward declaration: mosquitto.h stays in the .cpp

namespace station {

class MqttPublisher final : public Publisher {
public:
    MqttPublisher(const std::string& host, int port, const std::string& clientId = "bme280-station");
    ~MqttPublisher() override;

    MqttPublisher(const MqttPublisher&)            = delete;
    MqttPublisher& operator=(const MqttPublisher&) = delete;

    bool isConnected() const { return connected_; }

    /// QoS 0, not retained. Returns false when not connected or on error.
    bool publish(const std::string& topic, const std::string& payload) override;

private:
    static void onConnect(mosquitto* client, void* self, int rc);
    static void onDisconnect(mosquitto* client, void* self, int rc);

    mosquitto*        client_ = nullptr;
    std::atomic<bool> connected_{false};
};

} // namespace station
