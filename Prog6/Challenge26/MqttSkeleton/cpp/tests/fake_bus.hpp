/**
 * @file fake_bus.hpp
 * @brief In-memory IMessageBus for unit tests.
 *
 * Records what a component publishes and lets a test inject messages as if
 * they came from the broker. This is the test double the Unit Testing
 * workshop talks about: the device under test cannot tell the difference.
 */

#ifndef SIM_FAKE_BUS_HPP
#define SIM_FAKE_BUS_HPP

#include <string>
#include <vector>

#include "message_bus.hpp"
#include "topics.hpp"

namespace sim::test {

class FakeBus : public IMessageBus {
public:
    struct Published {
        std::string topic;
        std::string payload;
        bool retain;
    };

    void publish(const std::string& topic, const std::string& payload, bool retain) override {
        published.push_back({topic, payload, retain});
    }

    void subscribe(const std::string& filter, Handler handler) override {
        subscriptions.push_back({filter, std::move(handler)});
    }

    /// Deliver a message from "the broker" to every matching subscription.
    void deliver(const std::string& topic, const std::string& payload) {
        for (const auto& subscription : subscriptions) {
            if (topics::matches(subscription.filter, topic)) {
                subscription.handler(topic, payload);
            }
        }
    }

    const Published& last() const { return published.back(); }

    struct Subscription {
        std::string filter;
        Handler handler;
    };
    std::vector<Published> published;
    std::vector<Subscription> subscriptions;
};

} // namespace sim::test

#endif // SIM_FAKE_BUS_HPP
