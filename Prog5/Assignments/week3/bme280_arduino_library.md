# Assignment 3 - Sensor library (BME280), core + Arduino

*Introduced Thu 17 Sep 2026 - deadline Thu 24 Sep 2026 - tag `v0.3-arduino`*

Lecture 3 covered the Liskov Substitution Principle, lists, inline functions and default
parameters. This week you build the library that the rest of Prog 5 grows from.

## Goal

A C++ library for the Bosch **BME280** (temperature, pressure, humidity) that runs on your
Arduino / SAMD21 board **and** is structured so that next week it also runs on a Raspberry Pi
without touching the sensor code.

The starter in [`Code/week2/bme280_starter`](../../Code/week2/bme280_starter/bme280/) has the
structure and the Bosch driver already in place; the wrapper methods are marked `TODO`. You may
start from it or from scratch, but the structure below is mandatory.

## Structure

```
bme280lib/
├── include/bme280/   Bus.hpp (interface), Types.hpp, Bme280.hpp     <- the core: no Wire, no Linux
├── src/              Bme280.cpp                                     <- wraps the Bosch C driver
├── third_party/      Bosch BME280 SensorAPI, vendored unmodified
├── platform/arduino/ ArduinoI2cBus.hpp/.cpp (uses Wire)
├── examples/arduino/ sketch that prints T / P / H every second
├── library.properties
└── README.md
```

## Steps

1. **Vendor the Bosch driver** (`bme280.c`, `bme280.h`, `bme280_defs.h`, LICENSE) into
   `third_party/` (the starter already has them, from github.com/boschsensortec/BME280_SensorAPI).
   Do not edit it. It already is platform independent: it asks you for three
   function pointers (`read`, `write`, `delay_us`) and an `intf_ptr`.
2. **Define `Bus`**: an abstract class with `read(reg, data, len)`, `write(reg, data, len)` and
   `delayUs(us)`. Nothing else. Only `<cstdint>` and `<cstddef>` in this header.
3. **Write `Bme280`**: constructor takes a `Bus&`; three `static` member functions act as the C
   callbacks and use `intf_ptr` to get back to the bus. Public API: `init(config = Config{})`,
   `readForced(Measurement&)`, `read(Measurement&)`. Every method returns an `Error` enum, no
   exceptions, no negative ints.
4. **Default parameters, applied**: `init()` takes a `Config` with defaults for the datasheet's
   "weather monitoring" use case (forced mode, oversampling x1, filter off). The example sketch
   calls `init()` with no arguments; a second example overrides one field.
5. **`ArduinoI2cBus`**: implements `Bus` with `Wire`. This is the *only* file that includes
   `<Wire.h>` or `<Arduino.h>`.
6. **Example sketch** that prints one measurement per second over Serial.
7. **LSP check** (write it in the README): can every implementation of `Bus` be used wherever a
   `Bus&` is expected, without the sensor class noticing? What would break it (hint: a bus that
   returns `true` but writes nothing)?

## Hand in

* Repository with the structure above, tag `v0.3-arduino`.
* Serial output of the example sketch pasted into the README (or a screenshot).
* README section "Design": three sentences on why the core knows nothing about `Wire`.

## Pitfalls

* No `String`, `Serial` or `<iostream>` in `include/` or `src/`.
* No `new` / `malloc` in the core; an AVR has 2 kB of RAM.
* Compile without `BME280_DOUBLE_ENABLE` on AVR (integer compensation), or convert in the
  wrapper. Read the units of the Bosch integer output carefully (T in 0.01 °C, P in Pa, H in
  1/1024 %RH).
