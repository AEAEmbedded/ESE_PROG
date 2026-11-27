#include "mosquitto_bus.hpp"

#include <mosquitto.h>

#include <iostream>
#include <stdexcept>

#include "topics.hpp"

namespace sim {

namespace {
constexpr int kQos = 1;
constexpr int kKeepAliveSeconds = 30;
} // namespace

MosquittoBus::MosquittoBus(std::string clientId, std::string host, int port,
                           std::optional<LastWill> will)
    : host_(std::move(host)), port_(port) {
    mosquitto_lib_init();  // reference counted inside libmosquitto
    client_ = mosquitto_new(clientId.c_str(), true, this);
    if (client_ == nullptr) {
        throw std::runtime_error("mosquitto_new failed");
    }
    mosquitto_connect_callback_set(client_, &MosquittoBus::onConnect);
    mosquitto_message_callback_set(client_, &MosquittoBus::onMessage);
    if (will) {
        mosquitto_will_set(client_, will->topic.c_str(), static_cast<int>(will->payload.size()),
                           will->payload.data(), kQos, true);
    }
}

MosquittoBus::~MosquittoBus() {
    disconnect();
    mosquitto_destroy(client_);
    mosquitto_lib_cleanup();
}

void MosquittoBus::connect() {
    const int rc = mosquitto_connect(client_, host_.c_str(), port_, kKeepAliveSeconds);
    if (rc != MOSQ_ERR_SUCCESS) {
        throw std::runtime_error(std::string("connect to ") + host_ + " failed: " + mosquitto_strerror(rc));
    }
    mosquitto_loop_start(client_);
}

void MosquittoBus::disconnect() {
    mosquitto_disconnect(client_);
    mosquitto_loop_stop(client_, false);
}

void MosquittoBus::publish(const std::string& topic, const std::string& payload, bool retain) {
    const int rc = mosquitto_publish(client_, nullptr, topic.c_str(), static_cast<int>(payload.size()),
                                     payload.data(), kQos, retain);
    if (rc != MOSQ_ERR_SUCCESS) {
        std::cerr << "publish to " << topic << " failed: " << mosquitto_strerror(rc) << '\n';
    }
}

void MosquittoBus::subscribe(const std::string& filter, Handler handler) {
    {
        const std::lock_guard<std::mutex> lock(mutex_);
        subscriptions_.push_back({filter, std::move(handler)});
    }
    // If not connected yet this fails silently; onConnect subscribes everything.
    mosquitto_subscribe(client_, nullptr, filter.c_str(), kQos);
}

void MosquittoBus::onConnect(mosquitto* client, void* self, int reasonCode) {
    auto* bus = static_cast<MosquittoBus*>(self);
    if (reasonCode != 0) {
        std::cerr << "connect refused: " << mosquitto_connack_string(reasonCode) << '\n';
        return;
    }
    const std::lock_guard<std::mutex> lock(bus->mutex_);
    for (const auto& subscription : bus->subscriptions_) {
        mosquitto_subscribe(client, nullptr, subscription.filter.c_str(), kQos);
    }
}

void MosquittoBus::onMessage(mosquitto* /*client*/, void* self, const mosquitto_message* message) {
    auto* bus = static_cast<MosquittoBus*>(self);
    const std::string topic(message->topic);
    const std::string payload(static_cast<const char*>(message->payload),
                              static_cast<std::size_t>(message->payloadlen));

    std::vector<Handler> handlers;
    {
        const std::lock_guard<std::mutex> lock(bus->mutex_);
        for (const auto& subscription : bus->subscriptions_) {
            if (topics::matches(subscription.filter, topic)) {
                handlers.push_back(subscription.handler);
            }
        }
    }
    for (const auto& handler : handlers) {  // outside the lock: a handler may publish/subscribe
        handler(topic, payload);
    }
}

} // namespace sim
