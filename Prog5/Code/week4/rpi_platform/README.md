# week 4 - rpi_platform: the sensor library on a Raspberry Pi

The week-2 starter library (`Code/week2/bme280_starter`) talks to a `Bus`. This folder adds the
Linux implementation of that interface and a smoke test, and builds both with CMake.

```
include/rpi/LinuxI2cBus.hpp   Bus implementation on /dev/i2c-1 (RAII: open in ctor, close in dtor)
src/LinuxI2cBus.cpp
examples/rpi_smoke_test.cpp   composition root + main loop, one measurement per second
CMakeLists.txt                core (add_subdirectory) + platform + example: three targets
```

The layering is the point: `bme280` (core) -> `rpi_platform` (Linux) -> `rpi_smoke_test` (app).
Nothing in the core links against, includes, or knows about the two layers above it.

## Prepare the Pi (once)

```bash
sudo raspi-config            # Interface Options -> I2C -> enable, then reboot
sudo apt install i2c-tools cmake g++ git
sudo usermod -aG i2c $USER   # log out and in again
i2cdetect -y 1               # BME280 shows up at 76 or 77
```

Wiring (Pi header): 3V3 -> VIN, GND -> GND, SDA (pin 3, GPIO2) -> SDA, SCL (pin 5, GPIO3) -> SCL.
Leave SDO open or to GND for `0x76`, to 3V3 for `0x77`.

## Build and run

The starter's `third_party/bme280/` already contains the Bosch driver files (BSD-3-Clause,
see the README there); nothing to download.

```bash
cd Prog5/Code/week4/rpi_platform
cmake -B build
cmake --build build
./build/rpi_smoke_test          # or ./build/rpi_smoke_test 0x77
```

To build against your own library instead of the starter:

```bash
cmake -B build -DSENSOR_LIB_DIR=/home/pi/bme280lib
```

## What to look at

* `LinuxI2cBus::read()` does the register write and the data read in **one** `I2C_RDWR`
  transaction (repeated start). Compare with `ArduinoI2cBus`: `Wire.endTransmission(false)`
  is the same idea.
* `LinuxI2cBus::delayUs()` exists only because `Bus` demands it. Assignment 4 asks you to fix
  that (Interface Segregation): a `Clock` interface next to `Bus`.
* The smoke test never mentions `bme280.h`, and the core never mentions `<linux/i2c-dev.h>`.
  Draw the dependency arrows; that is your week-4 class diagram.
