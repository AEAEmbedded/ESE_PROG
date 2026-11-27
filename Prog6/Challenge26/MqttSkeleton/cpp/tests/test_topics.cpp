// Project and standard headers must come BEFORE CppUTest/TestHarness.h.
#include <string>

#include "topics.hpp"

#include <CppUTest/TestHarness.h>

using namespace sim::topics;

TEST_GROUP(Topics){};

TEST(Topics, BuildsDeviceTopics) {
    STRCMP_EQUAL("scenario/devices/thermo-01/command/body", deviceCommand("thermo-01", "body").c_str());
    STRCMP_EQUAL("scenario/devices/thermo-01/event/measured", deviceEvent("thermo-01", "measured").c_str());
    STRCMP_EQUAL("scenario/devices/thermo-01/status", deviceStatus("thermo-01").c_str());
    STRCMP_EQUAL("scenario/devices/thermo-01/command/+", deviceCommandFilter("thermo-01").c_str());
    STRCMP_EQUAL("scenario/control/start", control("start").c_str());
}

TEST(Topics, LastSegmentIsTheParameterOrEvent) {
    STRCMP_EQUAL("body", lastSegment("scenario/devices/thermo-01/command/body").c_str());
    STRCMP_EQUAL("start", lastSegment("scenario/control/start").c_str());
    STRCMP_EQUAL("single", lastSegment("single").c_str());
}

TEST(Topics, ExactFilterMatchesOnlyItself) {
    CHECK(matches("scenario/state/vitals", "scenario/state/vitals"));
    CHECK_FALSE(matches("scenario/state/vitals", "scenario/state/current"));
    CHECK_FALSE(matches("scenario/state", "scenario/state/vitals"));
    CHECK_FALSE(matches("scenario/state/vitals", "scenario/state"));
}

TEST(Topics, PlusMatchesExactlyOneLevel) {
    CHECK(matches("scenario/devices/+/status", "scenario/devices/thermo-01/status"));
    CHECK_FALSE(matches("scenario/devices/+/status", "scenario/devices/thermo-01/event/measured"));
    CHECK(matches("scenario/devices/+/event/+", "scenario/devices/aed-01/event/shock"));
    CHECK_FALSE(matches("scenario/devices/+/event/+", "scenario/devices/aed-01/event"));
}

TEST(Topics, HashMatchesTheRest) {
    CHECK(matches("scenario/#", "scenario/state/vitals"));
    CHECK(matches("scenario/#", "scenario/devices/thermo-01/command/body"));
    CHECK(matches("scenario/devices/#", "scenario/devices/thermo-01/status"));
    CHECK_FALSE(matches("scenario/devices/#", "scenario/state/vitals"));
    CHECK(matches("#", "anything/at/all"));
}
