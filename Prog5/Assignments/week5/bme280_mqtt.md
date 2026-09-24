# Assignment 5 - Out into the world: sampling thread, callbacks, MQTT

*Introduced Thu 1 Oct 2026 - deadline Thu 8 Oct 2026 - tag `v0.5-mqtt`*

Lecture 5: Dependency Inversion Principle, threads and callbacks, sequence diagrams.

## Goal

The Pi library from assignment 4 publishes its measurements to an MQTT broker, once per second,
from its own thread, and the sensor class still knows nothing about MQTT, threads or JSON.

Starter: [`Code/week5/mqtt_station`](../../Code/week5/mqtt_station/) - `Publisher` interface,
`ConsolePublisher`, `MqttPublisher` (libmosquitto), `Sampler` (thread + callback) and a
`main.cpp` that wires them together. It links against the week-2/week-4 starters; point it at
your own library.

## Steps

1. **`Sampler`**: owns a `std::thread` that calls `sensor.readForced()` every *period* and hands
   the `Measurement` to a callback (`std::function<void(const Measurement&)>`). `start()`,
   `stop()`, and a destructor that stops. The sampler depends on the sensor **interface**, not on
   `Bme280` (see step 4).
2. **`Publisher`** interface: `bool publish(const std::string& topic, const std::string& payload)`.
   Two implementations: `ConsolePublisher` (prints) and `MqttPublisher` (libmosquitto, connects
   in the constructor, disconnects in the destructor).
3. **Composition root.** `main()` is the only place where concrete classes meet:

   ```cpp
   rpi::LinuxI2cBus bus("/dev/i2c-1", 0x76);
   rpi::LinuxClock  sysClock;          // not `clock`: C already owns that name
   bme280::Bme280   sensor(bus, sysClock);
   MqttPublisher    publisher("localhost", 1883);
   Sampler          sampler(sensor, std::chrono::seconds(1));
   sampler.onMeasurement([&](const Measurement& m) { publisher.publish(topic, toJson(m)); });
   ```

   No `#ifdef`, no globals, no singletons. The lambda is the callback.
4. **Dependency Inversion, applied.** Introduce `EnvironmentSensor` (`init()`,
   `readForced(Measurement&)`) and let `Bme280` implement it. `Sampler` takes an
   `EnvironmentSensor&`. Now write a `FakeSensor` that returns a ramp and run the whole
   MQTT chain on your laptop without a Pi or a sensor. That is the test the interface buys you.
5. **Topics and payload**: `han/ese/<your-name>/bme280/state` with a JSON payload
   `{"t":21.37,"p":101325,"h":45.2}`. Verify with `mosquitto_sub -t 'han/ese/#' -v`.
6. **Threads: what can go wrong?** The callback runs on the sampler thread. In the README, name
   one shared piece of state in your program and say how you protect it (or why you do not need
   to). Bonus: make `stop()` return within one period even if the sensor is slow.

## Hand in

* Tag `v0.5-mqtt`.
* README: how to install and start `mosquitto` on the Pi, the topic scheme, a `mosquitto_sub`
  capture, the threading paragraph.
* A **sequence diagram** (Mermaid or PlantUML) of one cycle: `Sampler` -> `EnvironmentSensor`
  -> `Bus` -> back -> callback -> `Publisher` -> broker. Show the sampler thread's activation
  bar; show that `main` is not on it.

## Pitfalls

* Calling `sensor.readForced()` from two threads at once (sampler + main) is a data race; the
  sensor class is not thread-safe and does not need to be. Keep one owner.
* `MqttPublisher` must survive the broker being down: `publish()` returns `false`, the sampler
  keeps sampling.
* Do not put `sleep_for(1 s)` inside the sensor class. Timing is the sampler's job.
