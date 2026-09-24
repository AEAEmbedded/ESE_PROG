include(FetchContent)
FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG        v2.4.11
)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)   # doctest 2.4.11 still declares cmake_minimum_required(3.0)
FetchContent_MakeAvailable(doctest)

add_executable(bme280_tests test_bme280.cpp)
target_include_directories(bme280_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(bme280_tests PRIVATE bme280 doctest::doctest)

add_test(NAME bme280_tests COMMAND bme280_tests)
