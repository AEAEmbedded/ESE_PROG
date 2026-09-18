# Assignment 6 - Design on paper: class, package and sequence diagrams

*Introduced Thu 8 Oct 2026 - deadline Thu 15 Oct 2026 - tag `v0.6-design`*

Lecture 6: coupling and cohesion, polymorphism, composition and aggregation, packages.

## Goal

A technical design of the library and station you now have, plus one polymorphism step in the
code. The diagrams must match the code at tag `v0.6-design`; the reviewer will check.

## Steps

1. **Class diagram** of the whole thing: `Bus`, `Clock`, `EnvironmentSensor`, `Publisher` as
   `<<interface>>`; `Bme280`, `LinuxI2cBus`, `ArduinoI2cBus`, `LinuxClock`, `ArduinoClock`,
   `MqttPublisher`, `ConsolePublisher`, `Sampler`, `FakeSensor`, `MockBus`. Realizations,
   dependencies, and the right diamond on each "has-a":
   * `Sampler` -> `std::thread`: composition (the thread cannot outlive the sampler).
   * `Bme280` -> `Bus`: aggregation (the bus is created outside and lives longer).
   * `Bme280` -> `bme280_dev` (the Bosch struct): composition.
   Write next to each diamond *why* it is the one you chose.
2. **Package diagram** with four packages and the dependency arrows between them:
   `core` (sensor + interfaces), `platform` (`arduino`, `linux`), `app` (sampler, publishers,
   main), `tests`. Rule to verify: `core` has **no outgoing arrows**. Map the packages onto your
   folders and onto your CMake targets; they should be the same partition three times.
3. **Sequence diagrams**, two of them:
   * `Bme280::init()`: soft reset, chip-ID read, calibration read, config write, including the
     hop into the Bosch C code and the callback back into `Bus`.
   * One sample-and-publish cycle (refine the sketch from week 5).
4. **Polymorphism in code.** The station now holds `std::vector<EnvironmentSensor*>` (or
   `std::array` on the Arduino side) with your `Bme280` and a second sensor: a real one if you
   have it, otherwise `FakeSensor`. The sampler iterates over the vector; the publisher gets one
   message per sensor. No `dynamic_cast`, no `if (sensor is Bme280)`.
5. **Coupling and cohesion review.** For each class, one line: what is its single reason to
   change? For each dependency arrow in the package diagram: is it towards an abstraction?

## Hand in

* Tag `v0.6-design`; diagrams in `docs/` as Mermaid / PlantUML source **and** rendered image.
* README links to the diagrams and contains the coupling / cohesion table.

## Pitfalls

* A class diagram with twelve classes and no interfaces is a drawing of a problem.
* If your sequence diagram needs a lifeline called `Wire` or `ioctl` inside `core`, the code
  has a dependency it should not have.
