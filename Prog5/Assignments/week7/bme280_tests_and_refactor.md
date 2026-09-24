# Assignments 7 and 8 - Trustworthy: unit tests, lifecycle, refactor report

*Introduced Thu 15 Oct 2026 - deadline Thu 22 Oct 2026 - tag `v1.0`*

Lecture 7: Embedded SOLID, object lifecycle (constructors, destructors, RAII, `const`).

## Assignment 7 - unit tests on the host (no mocking framework yet)

The `Bus` / `Clock` split is what makes this possible: the sensor class can be tested on your
laptop against a hand-written fake. Use the `MockBus` from the week-2 starter (a 256-byte
register map plus a write log) and a `FakeClock` that only counts. Test framework: doctest
(fetched by CMake, as in the starter) or Catch2.

Required tests, all in `tests/`, all run by `ctest`:

1. `init()` returns `WrongChipId` when register `0xD0` is not `0x60`.
2. `init()` returns `BusFailure` when the bus fails the first read.
3. `read()` before `init()` returns `NotInitialised`.
4. `init()` succeeds with the correct chip ID and the calibration block pre-loaded.
5. `readForced()` asks the `Clock` for at least the measurement time of the current config.
6. **Compensation matches the datasheet's worked example**: load the calibration values and raw
   ADC values from the datasheet, expect the temperature / pressure / humidity printed there.
   This is the test that catches endianness, sign and macro errors.
7. `MqttPublisher` is *not* unit-tested (it needs a broker); explain in one line why, and what
   you test instead (`Sampler` with `FakeSensor` + a `RecordingPublisher`).

## Assignment 8 - review and refactor

Review your own library at tag `v0.6-design` against **DRY, KISS, SOLID, loose coupling /
strong cohesion**, and the lifecycle rules below. Refactor. Hand in a report
(`docs/refactor_report.md`) with, per finding: what, where (file:line at the old tag), which
principle, what you changed, and the commit that changed it.

One refactor is mandatory, because every library in this course has it coming:

**The second sensor.** Your `Bus`, `Clock` and `EnvironmentSensor` live in the namespace (and
folder, and CMake target) of your sensor: `bme280::Bus`. Now add a second sensor class, real
(SHT45, SHT31, a second BME280 on another bus) or `FakeSensor` if you have no hardware. It must
use the same `Bus` and implement the same `EnvironmentSensor`. Question: does `sht45::Sht45`
now depend on `bme280::`? If yes, the abstractions are in the wrong package.

1. Draw the package diagram *before* (sensor B depends on sensor A) and *after*.
2. Move what both sensors share (`Bus`, `Clock`, `EnvironmentSensor`, `Measurement`, `Error`)
   into a package of its own, for example `hal/` with namespace `hal`, with its own CMake
   target. Rule afterwards: `bme280` and `sht45` depend on `hal`; neither depends on the other;
   `hal` depends on nothing.
3. Decide, and write down, what stays sensor-specific: `Config` and the register map are
   BME280-only; is `Measurement` shared or does each sensor have its own?
4. The station's `vector<hal::EnvironmentSensor*>` from assignment 6 now holds two different
   classes from two packages. The `Sampler` and the publishers must not change at all; if they
   do, say why in the report.

Name the principles you applied (DIP: both sensors depend on the same abstraction; ISP: a
distance sensor would *not* implement `EnvironmentSensor`; SRP for the package: one reason to
change) and show the dependency arrows in the diagram. This is the difference between a
BME280 library and a sensor library.

Lifecycle checklist to review against:

* Every resource has an owner with a destructor: `/dev/i2c-1` closed by `LinuxI2cBus`, the
  thread joined by `Sampler`, the mosquitto client destroyed by `MqttPublisher`.
* Classes that own a resource are non-copyable (`= delete`); classes that are handed a
  reference do not `delete` it.
* Nothing in `core` calls `new`. Nothing in `core` uses `std::string` or `<iostream>`.
* `const`-correct: `measurementTimeUs() const`, `isInitialised() const`, `const Measurement&`
  in callbacks, `const Config&` parameters.
* Construction does not touch hardware (`Bme280(Bus&, Clock&)` is cheap); `init()` does.
  Reason: you can build the object in a global on Arduino before `setup()` runs.
* Embedded SOLID: which abstractions cost a vtable and is that acceptable on your MCU? One
  paragraph with numbers from `avr-size` / `arm-none-eabi-size` (flash and RAM, with and
  without the interfaces).

## Hand in

* Tag `v1.0`. `ctest` green on the host; the Arduino example still compiles; the Pi still
  publishes.
* `docs/refactor_report.md` and the test output in the README.
