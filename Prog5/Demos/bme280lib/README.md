# bme280lib - one sensor library, two platforms

A Bosch BME280 driver whose core knows neither Arduino nor Linux. The bus and the clock are
interfaces; the sketch or `main()` decides what is behind them.

```
src/Bus.hpp, Clock.hpp, Types.hpp, Bme280.hpp/.cpp   core: builds anywhere, no heap, no float inside
src/ArduinoI2cBus.hpp                                 Arduino: ArduinoI2cBus(TwoWire&, addr), ArduinoClock
src/LinuxI2cBus.hpp/.cpp                              Linux:   LinuxI2cBus("/dev/i2c-1", addr), LinuxClock
examples/Basic, examples/TwoBuses                     Arduino sketches (MicroMod STM32F405, STM32duino)
examples/rpi/main.cpp                                 Raspberry Pi
tests/                                                doctest on the host, MockBus + FakeClock
```

## Design decisions

* **The sensor is handed its bus.** `Bme280(Bus&, Clock&)`. No global `Wire`, no `Wire.begin()`
  inside the driver: whoever creates the bus owns it. Two sensors at `0x76` on two buses is
  two `ArduinoI2cBus` objects (`examples/TwoBuses`).
* **Bus and Clock are separate** (Interface Segregation). A bus moves bytes; only the sensor
  needs to wait. `MockBus` fakes registers, `FakeClock` counts microseconds.
* **Construction is free, `init()` talks to hardware.** So a global sensor object on Arduino is
  safe: `init()` runs in `setup()` after `Wire.begin()`, and it can fail with an `Error`.
* **Integer compensation** straight from datasheet 4.2.3. `RawMeasurement` is fixed point
  (0.01 degC, Q24.8 Pa, Q22.10 %RH); `Measurement` converts to float at the API boundary only.
* **No exceptions, no heap, `[[nodiscard]]` on everything that can fail.**
* **One burst read** of the 8 data registers; `ctrl_hum` written before `ctrl_meas` (the
  datasheet's ordering rule), `config` written in sleep mode only.

## Wiring

Both boards talk 3.3 V I2C. A typical BME280 breakout (GY-BME280, Adafruit, SparkFun) has
`VIN`/`VCC`, `GND`, `SCL`, `SDA` and often `CSB` and `SDO`:

| Breakout pin | Connect to | Why |
| --- | --- | --- |
| VIN / VCC | 3.3 V (Adafruit and SparkFun boards also accept 5 V on VIN) | bare modules are 3.3 V only |
| GND | GND | |
| SCL | SCL of the bus you inject | |
| SDA | SDA of the bus you inject | |
| CSB | 3.3 V, or leave open if the breakout pulls it up | high = I2C mode, low = SPI |
| SDO | GND -> address `0x76`, 3.3 V -> address `0x77` | the address bit; the bus object gets this value |

Pull-ups: the Pi has 1.8 kOhm on SDA1/SCL1 on-board, most breakouts have 10 kOhm; a bare module
on the STM32 needs 4.7 kOhm to 3.3 V on both lines.

### SparkFun MicroMod STM32F405 on the ATP carrier

The ATP carrier exposes two I2C buses. `examples/Basic` uses the Primary one; `examples/TwoBuses`
uses both. The pin mapping is confirmed by the STM32duino 3.x variant for this board
(`variant_MICROMOD_F405.h`): `Wire` = PB11 / PB10 when you select **SparkFun MicroMod STM32F405**
as the board, `SDA1` / `SCL1` = PB7 / PB6. With the generic F405RG board selected, `Wire` is
PB7 / PB6 instead, which is why the examples name the pins explicitly.

```
  ATP carrier header            STM32F405 pin   STM32duino object          BME280
  ------------------            -------------   -----------------          ------
  I2C (Primary)  SDA  ---------- PB11  (I2C2)    TwoWire(PB11, PB10)  ----> SDA
  I2C (Primary)  SCL  ---------- PB10  (I2C2)                         ----> SCL
  I2C1 (Secondary) SDA --------- PB7   (I2C1)    Wire (default)       ----> SDA  (second sensor)
  I2C1 (Secondary) SCL --------- PB6   (I2C1)                         ----> SCL
  3.3V ------------------------------------------------------------------> VIN, CSB
  GND -------------------------------------------------------------------> GND, SDO (0x76)
```

The Primary bus is also on the carrier's Qwiic connector, so a Qwiic BME280 needs no wiring
at all: plug it in and use `TwoWire(PB11, PB10)`.

### Raspberry Pi 400 (and any other Pi with the 40-pin header)

Same pins as every Pi, but on the Pi 400 the header sits on the back of the keyboard, has no
labels, and is mirrored compared to a Pi 4 lying flat: **pin 1 is at the top right when you look
at the back of the unit**, odd pins in the top row. Confirm before wiring:

```bash
pinout        # gpiozero draws the header for the exact model it runs on
```


```
  Pi header (physical pin)         BME280
  ------------------------         ------
  pin 1   3V3  --------------------> VIN, CSB
  pin 3   GPIO2 / SDA1  -----------> SDA          bus: /dev/i2c-1
  pin 5   GPIO3 / SCL1  -----------> SCL
  pin 6   GND  --------------------> GND, SDO (0x76)

  Pi 400, seen from the back (odd pins = top row, pin 1 on the right):

     ... 11  9   7   5   3   1        <- SCL1 = 5, SDA1 = 3, 3V3 = 1
     ... 12  10  8   6   4   2        <- GND = 6
```

A Pi 400 GPIO breakout / T-cobbler avoids counting pins on an unlabelled header.

Enable the bus once (`sudo raspi-config` -> Interface Options -> I2C, reboot), then check:

```bash
i2cdetect -y 1        # a "76" (or "77") in the grid
```

If `i2cdetect` shows the sensor but `init()` returns `WrongChipId`, you have a BMP280 (chip id
`0x58`), not a BME280.

## Arduino (STM32F405 MicroMod)

Copy or symlink this folder into `~/Documents/Arduino/libraries/` (or use `platformio.ini`).
The ATP carrier's Primary I2C is `TwoWire(PB11, PB10)`; Secondary is the default `Wire`.

```bash
arduino-cli compile --fqbn STMicroelectronics:stm32:SparkFun:pnum=MICROMOD_F405,usb=CDCgen examples/Basic
# generic variant works too: --fqbn STMicroelectronics:stm32:GenF4:pnum=GENERIC_F405RGTX,usb=CDCgen
# upload with a J-Link: add ,upload_method=jlinkMethod and --upload
```

## Raspberry Pi

One-shot (installs packages, enables I2C, builds, runs the tests and the smoke test):

```bash
scp -r bme280lib pi@pi400.local:~
ssh pi@pi400.local 'bash ~/bme280lib/tools/pi_setup.sh 0x77'   # address from i2cdetect
```

By hand:

```bash
sudo raspi-config     # Interface Options -> I2C
i2cdetect -y 1        # 76 or 77
cmake -B build && cmake --build build
./build/bme280_rpi 0x77
```

Qwiic-to-jumper cable on the Pi 400 header (see Wiring): black GND -> pin 6, red 3.3 V -> pin 1,
blue SDA -> pin 3, yellow SCL -> pin 5. Keep the SDO jumper as it is on the STM32 setup, so the
address stays `0x77`.

Without cmake (a fresh Raspberry Pi OS has g++ but no cmake), the whole thing is one compiler line:

```bash
g++ -std=c++17 -O2 -Wall -Wextra -Isrc src/Bme280.cpp src/LinuxI2cBus.cpp examples/rpi/main.cpp -o bme280_rpi -lpthread
```

Verified 20 Sep 2026 on a Pi 400 (Debian 13, aarch64): 10/10 tests pass, `bme280_rpi 0x77` reads
23.5 degC / 1022 hPa / 54 %RH once per second, Ctrl-C exits cleanly and closes the bus.

## Tests (any machine)

```bash
cmake -B build && cmake --build build && ctest --test-dir build --output-on-failure
```

The compensation test pins the arithmetic to the datasheet's worked example
(T = 25.08 degC, p = 100653 Pa). That one test catches endianness, sign and shift mistakes.
