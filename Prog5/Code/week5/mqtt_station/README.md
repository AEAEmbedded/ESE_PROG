# week 5 - mqtt_station: sampling thread, callback, MQTT publisher

Takes the Pi build from week 4 and pushes measurements to an MQTT broker, once per second, from
a dedicated thread. The sensor class does not change.

```
include/station/EnvironmentSensor.hpp  interface the station sees (init, readForced)
include/station/Bmp280Adapter.hpp      week-2 starter class -> EnvironmentSensor (delete once Bme280 implements it)
include/station/FakeSensor.hpp         ramp generator, no hardware needed
include/station/Publisher.hpp          interface: publish(topic, payload)
include/station/ConsolePublisher.hpp   prints
include/station/MqttPublisher.hpp/.cpp libmosquitto, connect in ctor, disconnect in dtor
include/station/Sampler.hpp/.cpp       std::thread + std::function callback, stop() joins
examples/station_main.cpp              composition root
```

Layers: `bmp280` (core) -> `rpi_platform` (Linux I2C) and `station` (threads, MQTT) -> `station_main`.
`station` links against the core only for the `Measurement` and `Error` types.

## Broker on the Pi

```bash
sudo apt install mosquitto mosquitto-clients libmosquitto-dev
sudo systemctl enable --now mosquitto
mosquitto_sub -h localhost -t 'han/ese/#' -v      # leave this running in a second terminal
```

Mosquitto 2.x only listens on localhost by default. To publish from a laptop to the Pi, add
`listener 1883` and `allow_anonymous true` to `/etc/mosquitto/conf.d/lab.conf` and restart.

## Build and run

```bash
cd Prog5/Code/week5/mqtt_station
cmake -B build && cmake --build build
./build/station_main                     # Pi: real sensor -> MQTT
./build/station_main --fake --console    # laptop: no hardware, no broker
```

## Threads: what to check

* `Sampler::run()` calls the callback on **its** thread. `MqttPublisher::publish()` is safe to
  call from there (mosquitto guards its queue); `ConsolePublisher` is safe because it does one
  `printf`. If your callback touches something `main()` also touches, you need a mutex or an
  atomic.
* `stop()` wakes the sleeping thread through the condition variable, so shutdown takes at most
  one sensor read, not one full period.
* `MqttPublisher` reconnects by itself (`mosquitto_reconnect_delay_set`); while disconnected
  `publish()` returns `false` and the sampler keeps going.
