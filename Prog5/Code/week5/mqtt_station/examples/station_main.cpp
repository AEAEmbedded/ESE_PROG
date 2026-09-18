// Composition root of the weather station: the one place where the concrete
// classes meet. Everything else talks to interfaces.
//
//   ./station_main                   real sensor on /dev/i2c-1, MQTT to localhost
//   ./station_main --fake            FakeSensor (works on a laptop)
//   ./station_main --console         print instead of MQTT
//   ./station_main --fake --console  no hardware, no broker
//
// Verify:  mosquitto_sub -h localhost -t 'han/ese/#' -v

#include "station/ConsolePublisher.hpp"
#include "station/FakeSensor.hpp"
#include "station/MqttPublisher.hpp"
#include "station/Sampler.hpp"

#ifdef STATION_HAS_HARDWARE
#include "bmp280/Bmp280.hpp"
#include "rpi/LinuxI2cBus.hpp"
#include "station/Bmp280Adapter.hpp"
#endif

#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdio>
#include <cstring>
#include <string>
#include <thread>

namespace {
std::atomic<bool> keepRunning{true};
void onSigint(int) { keepRunning = false; }

bool hasFlag(int argc, char** argv, const char* flag)
{
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], flag) == 0) return true;
    }
    return false;
}

std::string toJson(const bmp280::Measurement& m)
{
    char buf[64];
    std::snprintf(buf, sizeof buf, "{\"t\":%.2f,\"p\":%.0f}", m.temperatureC, m.pressurePa);
    return buf;
}
} // namespace

int main(int argc, char** argv)
{
    std::signal(SIGINT, onSigint);
    const bool useFake    = hasFlag(argc, argv, "--fake");
    const bool useConsole = hasFlag(argc, argv, "--console");
    const std::string topic = "han/ese/student/bme280/state";   // put your name here

    // --- sensor ---------------------------------------------------------------
#ifdef STATION_HAS_HARDWARE
    rpi::LinuxI2cBus bus("/dev/i2c-1", 0x76);
    bmp280::Bmp280   bmp(bus);
    station::Bmp280Adapter realSensor(bmp);
#endif
    station::FakeSensor fakeSensor;

    station::EnvironmentSensor* sensor = &fakeSensor;
#ifdef STATION_HAS_HARDWARE
    if (!useFake) {
        if (!bus.isOpen()) { std::fprintf(stderr, "cannot open /dev/i2c-1\n"); return 1; }
        sensor = &realSensor;
    }
#else
    if (!useFake) { std::fprintf(stderr, "no hardware in this build, using --fake\n"); }
#endif
    if (sensor->init() != bmp280::Error::None) {
        std::fprintf(stderr, "sensor init failed\n");
        return 2;
    }

    // --- publisher ----------------------------------------------------------
    station::ConsolePublisher console;
    station::MqttPublisher    mqtt("localhost", 1883);
    station::Publisher&       publisher = useConsole ? static_cast<station::Publisher&>(console)
                                                     : static_cast<station::Publisher&>(mqtt);

    // --- wiring: the callback is a lambda, the sampler never sees MQTT ----------
    station::Sampler sampler(*sensor, std::chrono::seconds(1));
    sampler.onMeasurement([&](const bmp280::Measurement& m) {
        if (!publisher.publish(topic, toJson(m))) {
            std::fprintf(stderr, "publish failed (broker down?)\n");
        }
    });

    sampler.start();
    while (keepRunning) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));   // main has nothing to do
    }
    sampler.stop();
    std::printf("\nbye\n");
    return 0;   // destructors: ~Sampler (joined already), ~MqttPublisher disconnects, ~LinuxI2cBus closes
}
