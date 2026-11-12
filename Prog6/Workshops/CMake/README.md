# CMake Workshop

**HAN University of Applied Sciences**
**Software Engineering - Programming 6**
**Johan Korten - v1.1 2026 (2026-2027)**

---

## Workshop Overview

This workshop teaches you how to use CMake, a modern cross-platform build system generator. You'll learn why build systems exist, the problems with traditional Makefiles, and how CMake solves these issues.

---

## 📦 Repository

**GitHub:** [https://github.com/AEAEmbedded/ESE_PROG](https://github.com/AEAEmbedded/ESE_PROG)

**Workshop Location:**
```
https://github.com/AEAEmbedded/ESE_PROG/tree/main/Workshops/CMake
```

### Getting the Code

```bash
git clone https://github.com/AEAEmbedded/ESE_PROG.git
cd ESE_PROG/Workshops/CMake/Source
```

---

### Prerequisites

- Basic C++ knowledge (Programming 4/5)
- CMake installed (version 3.20 or higher; 3.21+ for the presets in ChallengeSkeleton)
- A C++17 compatible compiler (g++, clang, or MSVC)

---

## Workshop Structure

The workshop progresses through 7 steps, each building on the previous one:

```
Source/
├── SimpleProject/          # Step 1: Single file compilation
├── SimpleLibrary/          # Step 2: Multiple files
├── BiggerLibrary/          # Step 3: Complex project (manual compilation)
├── BiggerLibraryMake/      # Step 4: Using Makefiles
├── BiggerLibraryCMake/     # Step 5: Using CMake
├── BiggerLibraryCMakeV2/   # Step 6: Modern CMake in 6 stages (libraries,
│                           #         FetchContent, professional setup, STM32)
└── ChallengeSkeleton/      # Step 7: Project skeleton for the final challenge
                            #         (core library + app + CppUTest tests)
```

For the live-demo flow of the lesson, see [`DEMO.md`](DEMO.md).

---

## Step 1: SimpleProject

**Goal:** Understand basic compilation

### Contents
- `main.cpp` - Simple program that prints a message

### Compile and Run
```bash
cd Source/SimpleProject
g++ main.cpp -o sensor_app
./sensor_app
```

### What You Learn
- Basic compilation command
- How a single file becomes an executable
- This is simple... but what happens with more files?

---

## Step 2: SimpleLibrary

**Goal:** See the complexity increase with multiple files

### Contents
- `main.cpp` - Uses the SHT45 sensor class
- `sht45.hpp` - Header file with class declaration
- `sht45.cpp` - Implementation file

### Compile and Run
```bash
cd Source/SimpleLibrary
g++ main.cpp sht45.cpp -o sensor_app
./sensor_app
```

### What You Learn
- Need to list all `.cpp` files
- Both source files must be compiled
- Still manageable... but getting tedious

---

## Step 3: BiggerLibrary

**Goal:** Experience the pain of complex manual compilation

### Contents
- `main.cpp` - Main application
- `sht45.cpp/hpp` - Temperature sensor
- `i2c.cpp/hpp` - I2C communication
- `logger.cpp/hpp` - Logging utility
- `config.cpp/hpp` - Configuration management
- `filter.cpp/hpp` - Signal filtering
- `include/` - All header files organized

### Compile and Run
```bash
cd Source/BiggerLibrary
g++ main.cpp sht45.cpp i2c.cpp logger.cpp config.cpp filter.cpp \
    -I./include -std=c++17 -Wall -O2 -o sensor_app
./sensor_app
```

### What You Learn
- 😫 **This is painful!**
- Long compilation command
- Easy to make mistakes
- Have to type this every time you want to rebuild
- What if you add a new file? Have to remember to update the command!

### The Problem
Typing long compilation commands manually is:
- **Error-prone** - Easy to forget a file
- **Time-consuming** - Repetitive typing
- **Not scalable** - Gets worse with more files
- **Not portable** - Different commands for different platforms

---

## Step 4: BiggerLibraryMake

**Goal:** See how Makefiles help... and their limitations

### Contents
Same files as BiggerLibrary, plus:
- `Makefile` - Build automation script

### Build and Run
```bash
cd Source/BiggerLibraryMake
make            # Build the project
make run        # Build and run
make clean      # Clean build artifacts
make help       # Show available commands
```

### What You Learn

#### ✅ Makefiles are Better Than Manual Commands
- Don't need to type long g++ commands
- Incremental compilation (only rebuilds changed files)
- Dependency tracking (rebuilds when headers change)
- Reusable commands (make, make clean, etc.)

#### ❌ But Makefiles Have Problems

1. **Repetitive Rules** - Look at the Makefile:
   ```makefile
   main.o: main.cpp $(HEADERS)
       $(CXX) $(CXXFLAGS) -c main.cpp

   sht45.o: sht45.cpp include/sht45.hpp
       $(CXX) $(CXXFLAGS) -c sht45.cpp

   # ... 4 more nearly identical rules!
   ```
   Every `.cpp` file needs its own rule!

2. **Manual Listing** - You must list:
   - All source files: `SOURCES = main.cpp sht45.cpp ...`
   - All object files: `OBJECTS = main.o sht45.o ...`
   - All dependencies: `main.o: main.cpp $(HEADERS)`

3. **Manual Dependency Tracking** - Have to specify which `.cpp` depends on which `.hpp`

4. **Platform-Specific** - This Makefile won't work on Windows without modification

5. **Hard-Coded Compiler** - Uses `g++` - what if someone has `clang++` or MSVC?

6. **Tedious Maintenance**:
   - Add a new `.cpp` file? Update 3+ places in the Makefile
   - Change a dependency? Update manually
   - Support Windows? Write a new Makefile

### The Question
**"There must be a better way!"** 🤔

---

## Step 5: BiggerLibraryCMake

**Goal:** See how CMake solves all these problems!

### Contents
Same files as BiggerLibrary, plus:
- `CMakeLists.txt` - CMake build configuration (35 lines vs 62 in Makefile!)

### Build and Run
```bash
cd Source/BiggerLibraryCMake
cmake -S . -B build      # Configure: -S source dir, -B build dir (out-of-source)
cmake --build build      # Build the project
./build/sensor_app       # Run the application
```

(The classic `mkdir build && cd build && cmake ..` does the same thing; the
`-S`/`-B` form is the modern one-liner and works from any directory.)

Or using make (if CMake generated Makefiles):
```bash
cd build
make
./sensor_app
```

### What You Learn

#### ✨ CMake is Amazing!

**Compare the files:**

**Makefile (62 lines):**
```makefile
OBJECTS = main.o sht45.o i2c.o logger.o config.o filter.o

main.o: main.cpp $(HEADERS)
    $(CXX) $(CXXFLAGS) -c main.cpp
sht45.o: sht45.cpp include/sht45.hpp
    $(CXX) $(CXXFLAGS) -c sht45.cpp
# ... many more rules
```

**CMakeLists.txt (35 lines):**
```cmake
set(SOURCES
    main.cpp
    sht45.cpp
    i2c.cpp
    logger.cpp
    config.cpp
    filter.cpp
)

add_executable(sensor_app ${SOURCES})
```

That's it! CMake figures out the rest!

#### CMake Benefits

1. **No Object File Listing** ✅
   - CMake automatically creates `.o` files
   - You just list the `.cpp` sources

2. **No Repetitive Rules** ✅
   - One `add_executable()` command
   - CMake generates all compilation rules

3. **Automatic Dependency Scanning** ✅
   - CMake scans `#include` statements
   - Automatically tracks header dependencies
   - Rebuilds when headers change

4. **Cross-Platform** ✅
   - Same `CMakeLists.txt` works on:
     - Linux (generates Makefiles)
     - macOS (generates Makefiles or Xcode projects)
     - Windows (generates Visual Studio projects or Ninja)
   - CMake detects available compilers automatically

5. **Modern Build Features** ✅
   - Out-of-source builds (keeps source directory clean)
   - Parallel compilation (faster builds)
   - IDE integration (CLion, VS Code, Visual Studio)
   - Package management integration (vcpkg, Conan)

6. **Easy Maintenance** ✅
   - Add new file? Just add filename to `SOURCES`
   - Change includes? CMake detects automatically
   - One simple configuration file

---

## Step 6: BiggerLibraryCMakeV2

**Goal:** From "it works" to professional, modern CMake

Six self-contained stages, each with its own README and `CMakeLists.txt`
(run `./copy_sources.sh` once to populate the stage directories with sources):

1. **stage1** - Basic CMake (functional, but old-style)
2. **stage2** - Modern CMake: target-specific commands
3. **stage3** - Library structure: modular, reusable libraries
4. **stage4** - External dependencies with `FetchContent`
5. **stage5** - Professional, production-ready configuration
6. **stage6** - Real-world STM32 embedded project

See [`Source/BiggerLibraryCMakeV2/README.md`](Source/BiggerLibraryCMakeV2/README.md).

---

## Step 7: ChallengeSkeleton

**Goal:** Take everything home - the starting point for your final challenge

A minimal, buildable project with the structure the final assignment needs:
a core-logic **library** (testable, no I/O), an **app** that links against it,
and **CppUTest unit tests** run via `ctest`, plus `CMakePresets.json` so the
whole team builds identically.

```bash
cd Source/ChallengeSkeleton
cmake -S . -B build          # Downloads CppUTest automatically (FetchContent)
cmake --build build
./build/app/skeleton_app
ctest --test-dir build --output-on-failure
```

Copy it as the start of your team repository. See
[`Source/ChallengeSkeleton/README.md`](Source/ChallengeSkeleton/README.md).
The Unit Testing workshop builds directly on this structure.

---

## Key Concepts

### Out-of-Source Builds

CMake uses a separate `build/` directory:
```
BiggerLibraryCMake/
├── CMakeLists.txt      # Configuration
├── main.cpp            # Source files
├── sht45.cpp
└── build/              # All build artifacts here
    ├── Makefile        # Generated
    ├── sensor_app      # Generated
    └── *.o             # Generated
```

**Benefits:**
- Source directory stays clean
- Can delete `build/` anytime to start fresh
- Can have multiple build configurations (Debug/Release)

### CMake Workflow

1. **Configure**: `cmake ..`
   - Reads `CMakeLists.txt`
   - Detects compiler, platform
   - Generates native build system (Makefile, .sln, etc.)

2. **Build**: `cmake --build .`
   - Runs the native build system
   - Compiles source files
   - Links executable

3. **Clean**: `rm -rf build/`
   - Delete build directory
   - Start fresh anytime

---

## Code Quality

All code in this workshop follows:
- ✅ **MISRA C++ Guidelines** - Safe, portable C++
- ✅ **Clean Code Principles** - Readable, maintainable
- ✅ **Bjarne Stroustrup's C++ Guidelines** - Modern C++ best practices

### Code Features

1. **RAII (Resource Acquisition Is Initialization)**
   ```cpp
   I2C i2c;        // ✅ Fully initialized, ready to use
   Config config;  // ✅ Config loaded in constructor
   ```
   Not:
   ```cpp
   I2C i2c;
   i2c.init();     // ❌ Two-phase initialization
   ```

2. **Rule of Five** - All classes explicitly declare copy/move operations

3. **Const Correctness** - Methods that don't modify state are `const`

4. **No Magic Numbers** - All literals have descriptive names:
   ```cpp
   static constexpr float SIMULATED_TEMPERATURE = 22.5F;
   ```

5. **Fixed-Width Types** - `uint8_t`, `int32_t` instead of `unsigned char`, `int`

6. **Documentation** - All classes and public methods documented

---

## Workshop Exercises

### Exercise 1: Build All Projects
Go through each step and build all 5 projects. Notice how the compilation gets more complex, then simpler with CMake.

### Exercise 2: Add a New Class
Add a new sensor class (e.g., `bmp280.cpp/hpp` for pressure sensor):
1. Create the files
2. Update BiggerLibraryMake's Makefile - notice how many places you need to edit
3. Update BiggerLibraryCMake's CMakeLists.txt - just add one line!

### Exercise 3: Cross-Platform Build
Try generating different build systems. Run these **from the project
directory** (the one containing `CMakeLists.txt`, e.g. `BiggerLibraryCMake/`)
and give each generator its own build directory, so the generator-mismatch
error cannot occur:
```bash
# Unix Makefiles (default on Linux/Mac)
cmake -S . -B build-make -G "Unix Makefiles"

# Ninja (fast build system)
cmake -S . -B build-ninja -G "Ninja"

# Xcode (macOS)
cmake -S . -B build-xcode -G "Xcode"
```

### Exercise 4: Debug vs Release
Build with different configurations (again from the project directory):
```bash
# Debug build (no optimization, debug symbols)
cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build-debug

# Release build (optimized)
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release
```

---

## Common CMake Commands

### Basic Commands
Run from the **project directory** (where `CMakeLists.txt` lives):
```bash
cmake -S . -B build         # Configure
cmake --build build         # Build
cmake --build build --clean-first  # Clean then build
```

(`cmake ..` is the same configure step, but only works from inside a `build/`
directory: `..` must point at the directory with `CMakeLists.txt`.)

### Configuration Options
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug       # Debug build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release     # Release build
cmake -S . -B build -DCMAKE_CXX_COMPILER=clang++   # Use clang instead of g++
```

### Generator Selection
```bash
cmake -S . -B build-ninja -G "Ninja"           # Use Ninja build system
cmake -S . -B build-make -G "Unix Makefiles"   # Use make
cmake -S . -B build-xcode -G "Xcode"           # Generate Xcode project
```

---

## Troubleshooting

### CMake not found
```bash
# macOS (using Homebrew)
brew install cmake

# Ubuntu/Debian
sudo apt-get install cmake

# Check version
cmake --version
```

### Build errors after changes
```bash
# Clean rebuild
cd build
rm -rf *
cmake ..
cmake --build .
```

### Wrong compiler detected
```bash
# Specify compiler explicitly
cmake -DCMAKE_CXX_COMPILER=g++-11 ..
```

### CMake Cache Errors (Common!)

**This is a very common issue students encounter!**

#### Error: Source Directory Mismatch

```
CMake Error: The source "/path/old/location/CMakeLists.txt" does not match
the source "/path/new/location/CMakeLists.txt" used to generate cache.
Re-run cmake with a different source directory.
```

**What happened:**
- You moved or renamed the project directory
- The build cache still references the old path
- CMake refuses to continue with mismatched paths

**Solution:**
```bash
# Delete and recreate the build directory
rm -rf build
mkdir build
cd build
cmake ..
```

#### Error: Generator Mismatch

```
CMake Error: Error: generator : Ninja
Does not match the generator used previously: Unix Makefiles
Either remove the CMakeCache.txt file and CMakeFiles directory
or choose a different binary directory.
```

**What happened:**
- First build used one generator (e.g., Unix Makefiles)
- Now trying to use a different generator (e.g., Ninja)
- Cache contains old generator configuration

**Solution:**
```bash
# Option 1: Clean build directory (recommended)
rm -rf build
mkdir build
cd build
cmake -G "Ninja" ..  # Or whatever generator you want

# Option 2: Delete cache files only
cd build
rm CMakeCache.txt
rm -rf CMakeFiles
cmake -G "Ninja" ..
```

#### Why This Happens

CMake caches configuration to speed up rebuilds:
- Detected compiler
- Generator choice (Make, Ninja, Visual Studio, etc.)
- Absolute paths to sources
- Configured options

When you change:
- Project location
- Generator
- CMake version
- Build system

The cache becomes invalid!

#### Prevention

**Best Practice:**
```bash
# When in doubt, delete the build directory!
rm -rf build
mkdir build
cd build
cmake ..
```

**Why this works:**
- Out-of-source builds keep source code clean
- Build directory is disposable
- No risk - your source code is never touched
- Fresh start = no cache issues

**Teaching Moment:** This is actually a **great feature** of out-of-source builds! If in-source builds (not recommended) got corrupted, you'd have a mess in your source directory. With out-of-source, just delete `build/` and start over!

---

## Summary: Why CMake?

| Feature | Manual | Makefile | CMake |
|---------|--------|----------|-------|
| Easy to use | ❌ Long commands | ⚠️ Better | ✅ Simple |
| Incremental builds | ❌ No | ✅ Yes | ✅ Yes |
| Dependency tracking | ❌ Manual | ⚠️ Manual | ✅ Automatic |
| Cross-platform | ❌ No | ❌ No | ✅ Yes |
| IDE integration | ❌ No | ⚠️ Limited | ✅ Excellent |
| Maintenance | ❌ Terrible | ⚠️ Tedious | ✅ Easy |
| Scalability | ❌ No | ⚠️ Limited | ✅ Excellent |

### The Evolution
1. **Manual compilation** - Simple but doesn't scale
2. **Makefiles** - Better, but platform-specific and repetitive
3. **CMake** - Modern, cross-platform, maintainable! ✨

---

## Further Learning

### Official Documentation
- [CMake Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)
- [CMake Documentation](https://cmake.org/documentation/)

### Best Practices
- [Modern CMake Guide](https://cliutils.gitlab.io/modern-cmake/)
- [Effective Modern CMake](https://gist.github.com/mbinna/c61dbb39bca0e4fb7d1f73b0d66a4fd1)

### C++ Resources
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/)
- [MISRA C++ 2023](https://misra.org.uk/)

---

## Questions?

If you have questions or issues during the workshop:
1. Check the troubleshooting section above
2. Ask your instructor
3. Consult the CMake documentation

---

**Happy Building!** 🛠️

---

*This workshop is part of the Software Engineering curriculum at HAN University of Applied Sciences. It builds on previous workshops about Clean Code and MISRA C++ guidelines.*
