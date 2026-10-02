#!/usr/bin/env bash
# One-shot setup + build + smoke test on a Raspberry Pi (tested target: Pi 400, Raspberry Pi OS).
#   scp -r bme280lib pi@<host>:~ && ssh pi@<host> 'bash ~/bme280lib/tools/pi_setup.sh'
set -euo pipefail
cd "$(dirname "$0")/.."

export PATH="$PATH:/usr/sbin"
echo "== packages (skipped when g++ and i2cdetect are already there; cmake is optional)"
if command -v g++ >/dev/null && command -v i2cdetect >/dev/null; then
    echo "g++ and i2cdetect present"
else
    # Visible output on purpose: a hang here is the network or the sudo password prompt.
    sudo apt-get install -y --no-install-recommends g++ i2c-tools cmake
fi

echo "== enable I2C (raspi-config, non-interactive; takes effect after a reboot if it was off)"
if ! ls /dev/i2c-1 >/dev/null 2>&1; then
    sudo raspi-config nonint do_i2c 0
    echo "I2C was off and is now enabled: sudo reboot, then run this script again."
    exit 0
fi
sudo usermod -aG i2c "$USER" || true

echo "== who is on the bus"
SCAN="$(i2cdetect -y 1)"
echo "$SCAN"
# The BME280 answers on 0x76 (SDO low) or 0x77 (SDO high). Take the one that is there;
# an explicit argument still wins.
ADDR="${1:-}"
if [ -z "$ADDR" ]; then
    if   echo "$SCAN" | grep -q '^70: .*\b77\b'; then ADDR=0x77
    elif echo "$SCAN" | grep -q '^70: .*\b76\b'; then ADDR=0x76
    else
        echo "no device at 0x76 or 0x77 on /dev/i2c-1: check wiring (SDA pin 3, SCL pin 5, 3V3 pin 1, GND pin 6)"
        exit 1
    fi
fi
echo "BME280 address: $ADDR"

echo "== build (core + Linux layer + bme280_rpi)"
mkdir -p build
if command -v cmake >/dev/null; then
    cmake -B build -DCMAKE_BUILD_TYPE=Release >/dev/null
    cmake --build build -j"$(nproc)"
    echo "== host tests"
    ctest --test-dir build --output-on-failure
else
    echo "no cmake: plain g++ (tests need cmake for doctest, skipped)"
    g++ -std=c++17 -O2 -Wall -Wextra -Isrc src/Bme280.cpp src/LinuxI2cBus.cpp examples/rpi/main.cpp -o build/bme280_rpi -lpthread
fi

echo "== smoke test at $ADDR (Ctrl-C to stop)"
./build/bme280_rpi "$ADDR"
