# Unit tests with CppUTest, fetched automatically. Same setup as the
# ChallengeSkeleton of the CMake workshop. No broker needed: the tests use
# a FakeBus (fake_bus.hpp) instead of MosquittoBus.
include(FetchContent)
FetchContent_Declare(
    CppUTest
    GIT_REPOSITORY https://github.com/cpputest/cpputest.git
    GIT_TAG master
)
set(TESTS OFF CACHE BOOL "")  # Don't build CppUTest's own tests
set(CPPUTEST_MEM_LEAK_DETECTION_DISABLED ON CACHE BOOL "")
FetchContent_MakeAvailable(CppUTest)

add_executable(test_mqtt_skeleton
    main.cpp
    test_topics.cpp
    test_payload.cpp
    test_virtual_thermometer.cpp
)
target_link_libraries(test_mqtt_skeleton PRIVATE sim_devices sim_bus CppUTest)
target_compile_options(test_mqtt_skeleton PRIVATE -Wall -Wextra -Wpedantic)

add_test(NAME mqtt_skeleton COMMAND test_mqtt_skeleton -c)
