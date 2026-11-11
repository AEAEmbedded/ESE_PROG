# Core library: pure logic, no I/O. Everything in here is unit-testable.
add_library(ews_core STATIC
    ews.cpp
)
target_include_directories(ews_core PUBLIC include)
target_compile_options(ews_core PRIVATE -Wall -Wextra -Wpedantic)
