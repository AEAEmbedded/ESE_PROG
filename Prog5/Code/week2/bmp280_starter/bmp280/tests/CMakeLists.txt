include(FetchContent)
FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG        v2.4.11
)
FetchContent_MakeAvailable(doctest)

add_executable(bmp280_tests test_bmp280.cpp)
target_include_directories(bmp280_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(bmp280_tests PRIVATE bmp280 doctest::doctest)

add_test(NAME bmp280_tests COMMAND bmp280_tests)
