# bme280 - C++ wrapper around the Bosch BME280 SensorAPI

Component of the weather station (use cases: Monitor Temperature,
Monitor Barometric Pressure, Monitor Humidity).

    include/bme280/      public API: Bus (interface), Types, Bme280
    src/                 wrapper implementation (stubs marked TODO)
    third_party/bme280/  Bosch C driver, vendored unmodified (BSD-3-Clause)
    tests/               doctest + MockBus, host-only
    examples/            platform Bus implementations + hardware smoke test

Build and test on the host:

    cmake -B build -DBME280_BUILD_TESTS=ON
    cmake --build build
    ctest --test-dir build

Implementation order: bridge (done) -> toBoschSettings() -> init() ->
configure() -> readForced() / read() -> measurementTimeUs() -> I2cBus + smoke test.

Bosch API entry points you need: bme280_init, bme280_set_sensor_settings,
bme280_set_sensor_mode, bme280_get_sensor_data, bme280_cal_meas_delay
(all in third_party/bme280/bme280.h). Compiled with BME280_32BIT_ENABLE:
temperature in 0.01 degC, pressure in Pa, humidity in 1/1024 %RH.
