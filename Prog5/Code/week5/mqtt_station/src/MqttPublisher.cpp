#include "station/MqttPublisher.hpp"

#include <mosquitto.h>

namespace station {

MqttPublisher::MqttPublisher(const std::string& host, int port, const std::string& clientId)
{
    mosquitto_lib_init();
    client_ = mosquitto_new(clientId.c_str(), /*clean_session=*/true, this);
    if (client_ == nullptr) {
        return;
    }
    mosquitto_connect_callback_set(client_, &MqttPublisher::onConnect);
    mosquitto_disconnect_callback_set(client_, &MqttPublisher::onDisconnect);
    mosquitto_reconnect_delay_set(client_, 1, 10, true);

    // Async connect + a background network thread: the constructor returns
    // immediately and reconnects are handled for us if the broker restarts.
    if (mosquitto_connect_async(client_, host.c_str(), port, /*keepalive=*/60) == MOSQ_ERR_SUCCESS) {
        mosquitto_loop_start(client_);
    }
}

MqttPublisher::~MqttPublisher()
{
    if (client_ != nullptr) {
        mosquitto_disconnect(client_);
        mosquitto_loop_stop(client_, /*force=*/false);
        mosquitto_destroy(client_);
    }
    mosquitto_lib_cleanup();
}

bool MqttPublisher::publish(const std::string& topic, const std::string& payload)
{
    if (client_ == nullptr || !connected_) {
        return false;
    }
    const int rc = mosquitto_publish(client_, nullptr, topic.c_str(),
                                     static_cast<int>(payload.size()), payload.data(),
                                     /*qos=*/0, /*retain=*/false);
    return rc == MOSQ_ERR_SUCCESS;
}

// --- C callbacks: `self` is the `this` we handed to mosquitto_new ------------
void MqttPublisher::onConnect(mosquitto*, void* self, int rc)
{
    static_cast<MqttPublisher*>(self)->connected_ = (rc == 0);
}

void MqttPublisher::onDisconnect(mosquitto*, void* self, int)
{
    static_cast<MqttPublisher*>(self)->connected_ = false;
}

} // namespace station
