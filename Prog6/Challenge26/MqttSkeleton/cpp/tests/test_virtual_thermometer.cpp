// Project and standard headers must come BEFORE CppUTest/TestHarness.h.
#include <string>

#include "fake_bus.hpp"
#include "payload.hpp"
#include "virtual_thermometer.hpp"

#include <CppUTest/TestHarness.h>

using sim::VirtualThermometer;
using sim::test::FakeBus;

namespace {
constexpr int64_t kNow = 1801234567;
int64_t fixedClock() { return kNow; }
} // namespace

TEST_GROUP(VirtualThermometer) {
    FakeBus bus;
    VirtualThermometer* thermometer = nullptr;

    void setup() override {
        thermometer = new VirtualThermometer(bus, "thermo-01", fixedClock);
        thermometer->start();
    }
    void teardown() override { delete thermometer; }
};

TEST(VirtualThermometer, StartSubscribesToItsCommandsAndAnnouncesConnected) {
    CHECK_EQUAL(1U, bus.subscriptions.size());
    STRCMP_EQUAL("scenario/devices/thermo-01/command/+", bus.subscriptions[0].filter.c_str());

    CHECK_EQUAL(1U, bus.published.size());
    STRCMP_EQUAL("scenario/devices/thermo-01/status", bus.last().topic.c_str());
    STRCMP_EQUAL(R"({"timestamp":1801234567,"connected":true,"battery":100,"type":"thermometer"})",
                 bus.last().payload.c_str());
    CHECK(bus.last().retain);
}

TEST(VirtualThermometer, TriggerWithoutValueDoesNotPublish) {
    CHECK_FALSE(thermometer->pullTrigger());
    CHECK_EQUAL(1U, bus.published.size());  // only the status from start()
    CHECK_FALSE(thermometer->reading().has_value());
}

TEST(VirtualThermometer, TriggerPublishesTheCommandedBodyTemperature) {
    bus.deliver("scenario/devices/thermo-01/command/body", R"({"timestamp":1,"value":38.5,"unit":"C"})");

    CHECK(thermometer->pullTrigger());
    STRCMP_EQUAL("scenario/devices/thermo-01/event/measured", bus.last().topic.c_str());
    STRCMP_EQUAL(R"({"timestamp":1801234567,"value":38.5,"unit":"C","mode":"body"})", bus.last().payload.c_str());
    CHECK_FALSE(bus.last().retain);
}

TEST(VirtualThermometer, ModesAreIndependent) {
    bus.deliver("scenario/devices/thermo-01/command/body", R"({"value":38.5})");
    bus.deliver("scenario/devices/thermo-01/command/surface", R"({"value":36.1})");

    thermometer->setMode(VirtualThermometer::Mode::Surface);
    CHECK(thermometer->pullTrigger());
    STRCMP_EQUAL(R"({"timestamp":1801234567,"value":36.1,"unit":"C","mode":"surface"})", bus.last().payload.c_str());

    thermometer->setMode(VirtualThermometer::Mode::Body);
    DOUBLES_EQUAL(38.5, *thermometer->reading(), 1e-9);
}

TEST(VirtualThermometer, IgnoresOtherDevicesUnknownParametersAndBadPayloads) {
    bus.deliver("scenario/devices/thermo-99/command/body", R"({"value":40.0})");
    bus.deliver("scenario/devices/thermo-01/command/rhythm", R"({"value":"vf"})");
    bus.deliver("scenario/devices/thermo-01/command/body", R"({"value":"broken"})");

    CHECK_FALSE(thermometer->reading().has_value());
}

TEST(VirtualThermometer, StopAnnouncesDisconnected) {
    thermometer->stop();
    STRCMP_EQUAL("scenario/devices/thermo-01/status", bus.last().topic.c_str());
    STRCMP_EQUAL(R"({"timestamp":1801234567,"connected":false,"reason":"shutdown"})", bus.last().payload.c_str());
    CHECK(bus.last().retain);
}
