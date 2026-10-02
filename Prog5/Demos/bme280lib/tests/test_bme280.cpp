#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "Bme280.hpp"
#include "Fakes.hpp"

using namespace bme280;
using bme280::test::FakeClock;
using bme280::test::MockBus;

namespace {

// Bosch datasheet worked example (BMP280 datasheet 3.11.3; the BME280 uses the
// same T and P formulas). Expected: T = 25.08 degC, p = 100653 Pa.
void loadDatasheetCalibration(MockBus& bus)
{
    auto le = [](int v) { return std::vector<uint8_t>{ static_cast<uint8_t>(v & 0xFF), static_cast<uint8_t>((v >> 8) & 0xFF) }; };
    std::vector<uint8_t> a;
    for (int v : { 27504, 26435, -1000,                          // T1..T3
                   36477, -10685, 3024, 2855, 140, -7, 15500, -14600, 6000 }) {   // P1..P9
        auto b = le(v); a.insert(a.end(), b.begin(), b.end());
    }
    a.push_back(0);      // 0xA0, unused
    a.push_back(75);     // H1
    bus.load(0x88, a);
    bus.load(0xE1, { 0x70, 0x01,          // H2 = 368
                     0x00,                // H3 = 0
                     0x14, 0x30, 0x1E,    // H4 = 0x14<<4 | 0x0 = 320, H5 = 0x1E<<4 | 0x3 = 483
                     0x1E });             // H6 = 30
}

void loadDatasheetRaw(MockBus& bus)
{
    // adc_P = 415148 = 0x655AC -> msb 0x65, lsb 0x5A, xlsb 0xC0
    // adc_T = 519888 = 0x7EED0 -> msb 0x7E, lsb 0xED, xlsb 0x00
    // adc_H = 0x8000 (mid-scale)
    bus.load(0xF7, { 0x65, 0x5A, 0xC0, 0x7E, 0xED, 0x00, 0x80, 0x00 });
}

struct Rig {
    MockBus   bus;
    FakeClock clock;
    Bme280    sensor{bus, clock};
};

} // namespace

TEST_CASE("construction touches no hardware")
{
    Rig r;
    CHECK(r.bus.reads() == 0);
    CHECK(r.bus.writes().empty());
    CHECK_FALSE(r.sensor.isInitialised());
}

TEST_CASE("init rejects a wrong chip id")
{
    Rig r;
    r.bus.reg(0xD0) = 0x58;   // a BMP280
    CHECK(r.sensor.init() == Error::WrongChipId);
    CHECK_FALSE(r.sensor.isInitialised());
}

TEST_CASE("init reports a failing bus")
{
    Rig r;
    r.bus.failReads();
    CHECK(r.sensor.init() == Error::BusFailure);
}

TEST_CASE("read before init is refused")
{
    Rig r;
    Measurement m{};
    CHECK(r.sensor.read(m) == Error::NotInitialised);
    CHECK(r.sensor.readForced(m) == Error::NotInitialised);
}

TEST_CASE("init resets, then writes ctrl_hum before ctrl_meas")
{
    Rig r;
    loadDatasheetCalibration(r.bus);
    REQUIRE(r.sensor.init() == Error::None);

    const auto& w = r.bus.writes();
    REQUIRE(w.size() == 4);
    CHECK(w[0].reg == 0xE0); CHECK(w[0].value == 0xB6);   // soft reset
    CHECK(w[1].reg == 0xF2);                              // ctrl_hum ...
    CHECK(w[2].reg == 0xF5);                              // config (sleep mode only)
    CHECK(w[3].reg == 0xF4);                              // ... before ctrl_meas
    CHECK((w[3].value & 0x03) == 0);                      // stays in sleep until forced
}

TEST_CASE("measurement time follows the datasheet formula")
{
    Rig r;
    loadDatasheetCalibration(r.bus);
    REQUIRE(r.sensor.init() == Error::None);              // x1 / x1 / x1
    CHECK(r.sensor.measurementTimeUs() == 1250 + 2300 + (2300 + 575) + (2300 + 575));

    Config c;
    c.temperature = Oversampling::X2;
    c.pressure    = Oversampling::X16;
    c.humidity    = Oversampling::Skip;
    REQUIRE(r.sensor.configure(c) == Error::None);
    CHECK(r.sensor.measurementTimeUs() == 1250 + 2 * 2300 + (16 * 2300 + 575));
}

TEST_CASE("readForced triggers, waits at least the measurement time, then reads one burst")
{
    Rig r;
    loadDatasheetCalibration(r.bus);
    loadDatasheetRaw(r.bus);
    REQUIRE(r.sensor.init() == Error::None);
    const auto waitedBefore = r.clock.totalUs();
    const auto readsBefore  = r.bus.reads();

    RawMeasurement raw{};
    REQUIRE(r.sensor.readForced(raw) == Error::None);

    CHECK(r.bus.writes().back().reg == 0xF4);
    CHECK((r.bus.writes().back().value & 0x03) == 0x01);              // forced
    CHECK(r.clock.totalUs() - waitedBefore >= r.sensor.measurementTimeUs());
    CHECK(r.bus.reads() - readsBefore == 2);                           // status + data burst
}

TEST_CASE("compensation matches the datasheet example")
{
    Rig r;
    loadDatasheetCalibration(r.bus);
    loadDatasheetRaw(r.bus);
    REQUIRE(r.sensor.init() == Error::None);

    RawMeasurement raw{};
    REQUIRE(r.sensor.read(raw) == Error::None);
    CHECK(raw.temperatureCenti == 2508);                              // 25.08 degC
    CHECK(raw.pressureQ24_8 / 256 == 100653);                         // Pa

    Measurement m{};
    REQUIRE(r.sensor.read(m) == Error::None);
    CHECK(m.temperatureC == doctest::Approx(25.08).epsilon(0.001));
    CHECK(m.pressurePa   == doctest::Approx(100653.0).epsilon(0.0001));
    CHECK(m.humidityPct  >= 0.0F);
    CHECK(m.humidityPct  <= 100.0F);
}

TEST_CASE("humidity is clamped to 0..100 %RH at the ADC extremes")
{
    Rig r;
    loadDatasheetCalibration(r.bus);
    REQUIRE(r.sensor.init() == Error::None);

    for (uint8_t msb : { uint8_t{0x00}, uint8_t{0xFF} }) {
        r.bus.load(0xF7, { 0x65, 0x5A, 0xC0, 0x7E, 0xED, 0x00, msb, msb });
        Measurement m{};
        REQUIRE(r.sensor.read(m) == Error::None);
        CHECK(m.humidityPct >= 0.0F);
        CHECK(m.humidityPct <= 100.0F);
    }
}

TEST_CASE("a bus failure during read is reported, not hidden")
{
    Rig r;
    loadDatasheetCalibration(r.bus);
    REQUIRE(r.sensor.init() == Error::None);
    r.bus.failReads();
    Measurement m{};
    CHECK(r.sensor.read(m) == Error::BusFailure);
}
