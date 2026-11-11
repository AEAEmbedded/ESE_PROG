# Unit tests with CppUTest, fetched automatically (see stage 4 of the
# workshop). Same setup as the Unit Testing workshop uses.
include(FetchContent)
FetchContent_Declare(
    CppUTest
    GIT_REPOSITORY https://github.com/cpputest/cpputest.git
    GIT_TAG master
)
set(TESTS OFF CACHE BOOL "")  # Don't build CppUTest's own tests
# CppUTest's malloc/new leak-detection macros break libc++ headers on
# macOS (std::realloc becomes a macro). Off here; the Unit Testing
# workshop covers leak detection separately.
set(CPPUTEST_MEM_LEAK_DETECTION_DISABLED ON CACHE BOOL "")
FetchContent_MakeAvailable(CppUTest)

add_executable(test_ews
    main.cpp
    test_ews.cpp
)
target_link_libraries(test_ews PRIVATE ews_core CppUTest)
target_compile_options(test_ews PRIVATE -Wall -Wextra -Wpedantic)

# Makes the test runnable via: ctest --test-dir build --output-on-failure
add_test(NAME ews COMMAND test_ews -c)
