#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "MockBus.hpp"
#include "bme280/Bme280.hpp"

using namespace bme280;
using bme280::test::MockBus;

TEST_CASE("init fails on wrong chip id")
{
    MockBus bus;
    bus.reg(MockBus::kRegChipId) = 0x00;
    Bme280 sensor(bus);

    CHECK(sensor.init() == Error::WrongChipId);
    CHECK_FALSE(sensor.isInitialised());
}

TEST_CASE("init fails when bus fails")
{
    MockBus bus;
    bus.failNextRead();
    Bme280 sensor(bus);

    CHECK(sensor.init() == Error::BusFailure);
}

TEST_CASE("read before init is rejected")
{
    MockBus bus;
    Bme280 sensor(bus);
    Measurement m;

    CHECK(sensor.read(m) == Error::NotInitialised);
}

TEST_CASE("init succeeds with correct chip id")
{
    MockBus bus;
    // TODO: pre-load calibration registers 0x88..0xA1 and 0xE1..0xE7
    Bme280 sensor(bus);

    CHECK(sensor.init() == Error::None);
    CHECK(sensor.isInitialised());
}

TEST_CASE("compensation matches datasheet example")
{
    // TODO: calibration + raw values from the datasheet worked example,
    //       expected T/P from the same page. This is the test that catches
    //       endianness, sign and macro errors.
}
