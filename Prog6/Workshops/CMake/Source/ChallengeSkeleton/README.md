# ChallengeSkeleton - From Workshop to Your Project

**HAN University of Applied Sciences**
**Software Engineering - Programming 6**
**Johan Korten - v1.0 2026**

---

## What This Is

A minimal, buildable project structure you can copy as the starting point for
your final challenge repository. It puts everything from the CMake workshop
together:

```
ChallengeSkeleton/
├── CMakeLists.txt          # Root: project settings, subdirectories, testing
├── CMakePresets.json       # Shared build configurations for the whole team
├── core/                   # Pure logic as a library (testable, no I/O)
│   ├── CMakeLists.txt
│   ├── include/ews.hpp
│   └── ews.cpp
├── app/                    # The executable, links against core
│   ├── CMakeLists.txt
│   └── main.cpp
└── tests/                  # CppUTest unit tests, run via ctest
    ├── CMakeLists.txt
    ├── main.cpp
    └── test_ews.cpp
```

**Why this split matters for the challenge:** the rubric asks for automated
unit tests of your core logic (transitions, timing, early warning score).
That only works if the core logic lives in a **library without I/O**, so the
test executable can link against it. Keep MQTT, GUI and hardware out of
`core/`.

---

## About the Example Code

The `EarlyWarningScore` class is a deliberately simplified stand-in; it is
**not** the real Dutch PEWS. It scores one parameter (heart rate) against a
band table that is passed in at construction. The structure is the point:

- Thresholds come from **configuration**, not from code. The challenge
  explicitly counts hard-coded thresholds as an Open-Closed violation.
- Pure logic without I/O is trivially unit-testable - see `tests/test_ews.cpp`.

For your project, replace it with the real Dutch PEWS (age bands, work of
breathing, respiratory rate, heart rate, capillary refill time) read from a
configuration table: <https://dutchpews.com/wat-is-dutch-pews/>

---

## Build and Test

```bash
cmake -S . -B build          # Configure (downloads CppUTest automatically)
cmake --build build          # Build library, app and tests
./build/app/skeleton_app     # Run the demo app
ctest --test-dir build --output-on-failure   # Run the unit tests
```

The first configure takes a minute: `FetchContent` downloads and builds
CppUTest (see workshop stage 4). After that, builds are incremental.

### With Presets (recommended for teams)

`CMakePresets.json` ensures every team member builds with the same settings -
no more "works on my machine" (requires CMake 3.21+):

```bash
cmake --preset debug         # Configure into build/debug
cmake --build --preset debug
ctest --preset debug
```

---

## Using This for Your Project

1. Copy this directory as the starting point of your team repository.
2. Rename the project and targets in the `CMakeLists.txt` files.
3. Grow `core/` into your scenario engine; add one test file per class.
4. Add more subdirectories the same way (`add_subdirectory`), e.g. for the
   MQTT layer or a virtual SimDevice.

Next workshop (Unit Testing) builds directly on this skeleton: we will hang
real tests, test doubles and mocks into exactly this structure.
