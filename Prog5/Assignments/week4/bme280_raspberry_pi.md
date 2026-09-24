# Assignment 4 - Same sensor library, second platform: Raspberry Pi

*Introduced Thu 24 Sep 2026 - deadline Thu 1 Oct 2026 - tag `v0.4-pi`*

Lecture 4: Interface Segregation Principle, interfaces and abstract classes, UML dependencies.
Everything this week is applied to the library from assignment 3.

## Goal

The **same core** (`include/`, `src/`, `third_party/`) now also runs on a Raspberry Pi, talking
to the BME280 over `/dev/i2c-1`. Not a single line in the core changes to make that happen.
If it does, that is the finding of this week and it goes in your README.

Starter: [`Code/week4/rpi_platform`](../../Code/week4/rpi_platform/) - a `LinuxI2cBus`, a
smoke-test program and a CMake file that builds the week-2 starter library on the Pi.

## Steps

0. **Make what you inject an abstraction.** After Lab 3 your sensor probably receives an
   `I2CHelper` (wrapping a `TwoWire*`) through its constructor: injected, but a concrete Arduino
   type. Define the `Bus` interface, let `I2CHelper` (or a new `ArduinoI2cBus`) implement it, and
   give the sensor a `Bus&` instead. If your sensor still calls `Wire` directly, move those calls
   into that class first. The sensor never calls `Wire.begin()`: whoever owns the bus (the sketch, later
   `main()`) starts it, the sensor only uses it. Check: the sensor class compiles without
   `<Wire.h>`. The rest of this assignment assumes that split exists.
1. **Build the core on the Pi with CMake.** Add a `CMakeLists.txt` at the root of your library
   with three targets: the Bosch C code, your wrapper library, and (later) tests. The wrapper
   target must build on the Pi *and* on your laptop, because it contains no platform code.
   Prove it: `cmake -B build && cmake --build build` on both.
2. **`LinuxI2cBus`**: implements `Bus` with `open("/dev/i2c-1")`, `ioctl(I2C_SLAVE)` /
   `I2C_RDWR`, `read`, `write`, `close`. Opening happens in the constructor, closing in the
   destructor (this is RAII; week 7 comes back to it). Non-copyable: two objects sharing one
   file descriptor is a bug waiting to happen.
3. **Interface Segregation, applied.** Look at `Bus::delayUs()`. A bus moves bytes; why does it
   know about time? A `MockBus` in a test is forced to implement a delay it does not care about,
   and an `ArduinoI2cBus` and a `LinuxI2cBus` copy the same three lines with different sleep
   calls. Split it:

   ```cpp
   class Bus   { virtual bool read(...) = 0; virtual bool write(...) = 0; };
   class Clock { virtual void delayUs(uint32_t us) = 0; };
   ```

   `Bme280(Bus&, Clock&)`. Provide `ArduinoClock`, `LinuxClock` and (for tests) a `FakeClock`
   that only counts. Write in the README what got simpler because of the split.
4. **Smoke test on the Pi**: a program that initialises the sensor, prints one measurement per
   second and exits cleanly on Ctrl-C. Check with `i2cdetect -y 1` first that the sensor is at
   `0x76` or `0x77`.
5. **UML dependencies.** Draw a class diagram (Mermaid or PlantUML in the README) that shows:
   `<<interface>> Bus`, `<<interface>> Clock`, `Bme280`, `ArduinoI2cBus`, `LinuxI2cBus`,
   `ArduinoClock`, `LinuxClock`. Use realization arrows for "implements" and dependency arrows
   (dashed) for "uses". The diagram must make one thing obvious: **no arrow leaves the core
   towards a platform.** If you cannot draw it that way, your code has the problem, not the
   diagram.

## Hand in

* Tag `v0.4-pi`; the tag from week 3 still builds for Arduino.
* Layout: `platform/arduino/`, `platform/linux/`, `examples/arduino/`, `examples/rpi/`.
* README: build instructions for both targets, the class diagram, the ISP paragraph, and the
  terminal output of the smoke test on the Pi.

## Pitfalls

* `linux` is a predefined macro in GCC. `namespace linux` will not compile; use `rpi` or
  `platform_linux`.
* `I2C_RDWR` message lengths are 16-bit; guard `size_t len` before casting.
* If `init()` returns `WrongChipId` but `i2cdetect` sees the device, your `read()` probably
  does a STOP between the register write and the read; use one `I2C_RDWR` transaction with two
  messages (repeated start).
* If the smoke test works but the Arduino build is now broken, you put platform code in the core.
