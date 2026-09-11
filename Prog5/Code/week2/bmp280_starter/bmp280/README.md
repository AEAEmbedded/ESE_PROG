# bmp280 — C++ wrapper around the Bosch BMP2 SensorAPI

Component of the weather station (use cases: Monitor Temperature,
Monitor Barometric Pressure).

    include/bmp280/   public API: Bus (interface), Types, Bmp280
    src/              wrapper implementation (stubs marked TODO)
    third_party/bmp2/ Bosch C driver, vendored unmodified
    tests/            doctest + MockBus, host-only
    examples/         platform Bus implementations + hardware smoke test

Build and test on the host:

    cmake -B build -DBMP280_BUILD_TESTS=ON
    cmake --build build
    ctest --test-dir build

Implementation order: bridge → init() → readForced() → Error mapping →
Config enums → I2cBus + smoke test.
