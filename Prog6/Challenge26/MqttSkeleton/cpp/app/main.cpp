/**
 * virtual_thermometer - run a virtual thermometer SimDevice against a broker.
 *
 *   ./virtual_thermometer [host] [device-id]      (defaults: localhost thermo-02)
 *   Enter   pull the trigger (measure)
 *   b / s   body / surface mode
 *   q       quit
 *
 * This is the only place where the pieces are wired together: the device
 * class gets a MosquittoBus here, and a FakeBus in the tests.
 */

#include <iostream>
#include <string>

#include "mosquitto_bus.hpp"
#include "payload.hpp"
#include "topics.hpp"
#include "virtual_thermometer.hpp"

int main(int argc, char* argv[]) {
    const std::string host = argc > 1 ? argv[1] : "localhost";
    const std::string deviceId = argc > 2 ? argv[2] : "thermo-02";

    try {
        sim::MosquittoBus bus(
            "virtual-" + deviceId, host, 1883,
            sim::MosquittoBus::LastWill{
                sim::topics::deviceStatus(deviceId),
                sim::Payload().add("timestamp", sim::unixNow()).add("connected", false).add("reason", "last will").str()});
        sim::VirtualThermometer thermometer(bus, deviceId, sim::unixNow);

        bus.connect();
        thermometer.start();
        std::cout << "Connected to " << host << " as " << deviceId
                  << ". Enter = pull trigger, b/s = body/surface mode, q = quit\n";

        std::string line;
        while (std::getline(std::cin, line)) {
            if (line == "q") {
                break;
            }
            if (line == "b") {
                thermometer.setMode(sim::VirtualThermometer::Mode::Body);
            } else if (line == "s") {
                thermometer.setMode(sim::VirtualThermometer::Mode::Surface);
            } else if (thermometer.pullTrigger()) {
                std::cout << "measured " << *thermometer.reading() << " C ("
                          << sim::VirtualThermometer::toString(thermometer.mode()) << ")\n";
            } else {
                std::cout << "no " << sim::VirtualThermometer::toString(thermometer.mode())
                          << " temperature received from the engine yet\n";
            }
        }
        thermometer.stop();
    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
