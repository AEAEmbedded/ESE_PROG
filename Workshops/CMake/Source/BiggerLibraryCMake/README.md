# BiggerLibraryCMake - Step 5

**CMake Workshop - HAN University**
**The Modern Solution: CMake**

---

## What This Is

Same project as BiggerLibrary and BiggerLibraryMake, but now with **CMake**! This is **Step 5** - the finale of the CMake workshop, showing you the modern, elegant solution.

---

## Files

```
BiggerLibraryCMake/
├── README.md              # This file
├── CMakeLists.txt        # CMake configuration (35 lines!)
├── .gitignore            # Ignore build artifacts
├── include/              # All header files
│   ├── sht45.hpp
│   ├── i2c.hpp
│   ├── logger.hpp
│   ├── config.hpp
│   └── filter.hpp
├── main.cpp
├── sht45.cpp
├── i2c.cpp
├── logger.cpp
├── config.cpp
└── filter.cpp
```

---

## What You'll Learn

- ✅ CMake is **simple** to write
- ✅ CMake is **cross-platform**
- ✅ CMake **generates** build systems
- ✅ CMake has **automatic** dependency tracking
- ✅ CMake integrates with **modern IDEs**
- ✅ This is how **professionals** build C++ projects!

---

## How to Build

### The CMake Way (Out-of-Source Build)

```bash
# Create build directory (keeps source clean!)
mkdir build
cd build

# Configure (generate build system)
cmake ..

# Build
cmake --build .
```

**Or if you prefer make:**
```bash
cd build
make
```

### Run the Program

```bash
./sensor_app
```

**Expected Output:**
```
[LOG] Starting sensor application...
Temperature: 20.25
[LOG] Application complete
```

---

## The Amazing Truth ✨

### Compare File Sizes:

**Makefile (Step 4):**
- 62 lines
- Manual file listing
- Repetitive rules
- Platform-specific

**CMakeLists.txt (Step 5):**
- **35 lines** (almost half!)
- Automatic dependency detection
- No repetitive rules
- Cross-platform!

Let's look at the key parts:

```cmake
# That's it! Just list the sources:
set(SOURCES
    main.cpp
    sht45.cpp
    i2c.cpp
    logger.cpp
    config.cpp
    filter.cpp
)

# CMake figures out everything else!
add_executable(sensor_app ${SOURCES})
```

**No object file listing. No repetitive rules. Just simple, declarative configuration!**

---

## CMake Advantages

### ✅ 1. Simple Configuration

**Makefile:** 62 lines with 6 repetitive rules
**CMakeLists.txt:** 35 lines with ONE `add_executable()` call

### ✅ 2. Cross-Platform

**Same CMakeLists.txt works on:**
- Linux (generates Makefiles)
- macOS (generates Makefiles or Xcode projects)
- Windows (generates Visual Studio projects or Ninja)

**One configuration file, all platforms!**

### ✅ 3. Automatic Dependency Detection

CMake scans `#include` statements automatically:
- Knows which `.cpp` files depend on which `.hpp` files
- Rebuilds only what's needed when headers change
- **You don't maintain dependencies manually!**

### ✅ 4. No Repetitive Rules

**Makefile needed:**
```makefile
main.o: main.cpp $(HEADERS)
    $(CXX) $(CXXFLAGS) -c main.cpp
sht45.o: sht45.cpp include/sht45.hpp
    $(CXX) $(CXXFLAGS) -c sht45.cpp
# ... 4 more rules
```

**CMake needs:**
```cmake
add_executable(sensor_app ${SOURCES})
```

**That's it!**

### ✅ 5. Compiler Detection

CMake automatically finds and uses:
- `g++` on Linux
- `clang++` on macOS
- MSVC on Windows
- Or whatever you specify

**No hard-coding compilers!**

### ✅ 6. IDE Integration

**Modern IDEs understand CMake natively:**
- CLion
- VS Code (with CMake extension)
- Visual Studio
- Qt Creator
- Xcode

**Better code completion, debugging, and project navigation!**

### ✅ 7. Out-of-Source Builds

Build artifacts go in `build/` directory:
```
BiggerLibraryCMake/
├── CMakeLists.txt         # Source
├── main.cpp               # Source
└── build/                 # All build artifacts here
    ├── sensor_app         # Executable
    ├── *.o                # Object files
    └── Makefile           # Generated
```

**Benefits:**
- Source directory stays clean
- Can delete `build/` anytime
- Can have multiple build configurations (Debug/Release)
- Git can ignore `build/` easily

---

## Understanding CMakeLists.txt

Let's break down the file:

### 1. Minimum Version
```cmake
cmake_minimum_required(VERSION 3.10)
```
Ensures CMake has required features.

### 2. Project Declaration
```cmake
project(SensorApp VERSION 1.0 LANGUAGES CXX)
```
Names the project and specifies C++ language.

### 3. C++ Standard
```cmake
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
```
Portable way to request C++17 (works with all compilers).

### 4. Compiler Flags
```cmake
add_compile_options(-Wall -Wextra -O2)
```
Same flags as Makefile, but works across platforms.

### 5. Include Directories
```cmake
include_directories(include)
```
Tell compiler where headers are.

### 6. Source Files
```cmake
set(SOURCES
    main.cpp
    sht45.cpp
    i2c.cpp
    logger.cpp
    config.cpp
    filter.cpp
)
```
Just list them - no manual `.o` file listing!

### 7. Create Executable
```cmake
add_executable(sensor_app ${SOURCES})
```
**This one line does everything:**
- Creates compilation rules for all sources
- Detects header dependencies automatically
- Generates object files
- Links them into executable
- Sets up incremental builds

### 8. Status Messages
```cmake
message(STATUS "Building ${PROJECT_NAME} version ${PROJECT_VERSION}")
message(STATUS "C++ Standard: C++${CMAKE_CXX_STANDARD}")
```
Helpful output during configuration.

---

## CMake Workflow

### Step 1: Configure
```bash
cmake ..
```

**What happens:**
1. Reads `CMakeLists.txt`
2. Detects system (OS, compiler, tools)
3. Scans source files for dependencies
4. Generates native build system (Makefile, .sln, etc.)

**Output:**
```
-- The CXX compiler identification is AppleClang 15.0.0
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Building SensorApp version 1.0
-- C++ Standard: C++17
-- Configuring done
-- Generating done
-- Build files written to: .../build
```

### Step 2: Build
```bash
cmake --build .
```

**Or:**
```bash
make  # If CMake generated Makefiles
```

**What happens:**
1. Runs the generated build system
2. Compiles source files
3. Links executable
4. Incremental builds (only changed files)

### Step 3: Run
```bash
./sensor_app
```

---

## Common CMake Commands

### Basic Workflow
```bash
# Configure
cmake ..

# Build
cmake --build .

# Clean and rebuild
cmake --build . --clean-first
```

### Build Configurations
```bash
# Debug build (no optimization, debug symbols)
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Release build (optimized)
cmake -DCMAKE_BUILD_TYPE=Release ..
```

### Generator Selection
```bash
# Use Unix Makefiles (default on Linux/Mac)
cmake -G "Unix Makefiles" ..

# Use Ninja (fast build system)
cmake -G "Ninja" ..

# Use Xcode (macOS)
cmake -G "Xcode" ..
```

### Compiler Selection
```bash
# Use clang instead of g++
cmake -DCMAKE_CXX_COMPILER=clang++ ..

# Use specific version
cmake -DCMAKE_CXX_COMPILER=g++-11 ..
```

---

## Troubleshooting

### Problem: CMake Cache Errors

**Error message:**
```
CMake Error: The source "..." does not match the source "..." used to generate cache.
```

**Or:**
```
CMake Error: Error: generator : Ninja
Does not match the generator used previously: Unix Makefiles
```

**What happened:**
- The build directory has cached configuration from a previous run
- You changed something (moved project, changed generator, etc.)
- CMake detects mismatch and refuses to continue

**Solution:**
```bash
# Option 1: Delete build directory (recommended)
rm -rf build
mkdir build
cd build
cmake ..

# Option 2: Delete cache only
cd build
rm CMakeCache.txt
rm -rf CMakeFiles
cmake ..
```

**Why this happens:**
- Out-of-source builds cache configuration
- Cache includes absolute paths, generator choice, compiler info
- Changing any of these requires reconfiguration

**Prevention:**
- When in doubt, just delete and recreate `build/`
- That's the beauty of out-of-source builds!
- Your source code is never touched

### Problem: Wrong Compiler Detected

**Fix:**
```bash
cmake -DCMAKE_CXX_COMPILER=/path/to/compiler ..
```

### Problem: Can't Find Headers

**Check CMakeLists.txt:**
```cmake
include_directories(include)  # Make sure this is correct
```

### Problem: Linker Errors

**Make sure all sources listed:**
```cmake
set(SOURCES
    main.cpp
    sht45.cpp
    # ... make sure you didn't forget any!
)
```

---

## Exercise: Add a New File

Let's see how easy it is with CMake!

### Step 1: Create Files

Create `humidity.hpp` and `humidity.cpp`

### Step 2: Update CMakeLists.txt

**Add ONE line:**
```cmake
set(SOURCES
    main.cpp
    sht45.cpp
    i2c.cpp
    logger.cpp
    config.cpp
    filter.cpp
    humidity.cpp    # ← Just add this!
)
```

**That's it!** No need to:
- Create object file rule
- List dependencies
- Add to object file variable
- Write compilation command

### Step 3: Rebuild

```bash
cmake --build .
```

CMake automatically:
- Detects the new file
- Creates compilation rule
- Scans for dependencies
- Builds it

**Compare with Makefile:**
- Makefile: Edit 4+ places
- CMake: Edit 1 line

---

## Comparison Summary

| Feature | Manual | Makefile | CMake |
|---------|--------|----------|-------|
| Command | 😫 Very long | ✅ `make` | ✅ `cmake --build .` |
| Lines of config | 0 | 62 | **35** |
| Incremental builds | ❌ No | ✅ Yes | ✅ Yes |
| Dependency tracking | ❌ Manual | ⚠️ Manual | ✅ **Automatic** |
| Cross-platform | ❌ No | ❌ No | ✅ **Yes** |
| Repetitive | ❌ Very | ⚠️ Somewhat | ✅ **None** |
| Add new file | Edit command | Edit 4+ places | Edit 1 line |
| IDE integration | ❌ No | ⚠️ Limited | ✅ **Excellent** |
| Maintenance | 😫 Terrible | ⚠️ Tedious | ✅ **Easy** |
| Professional | ❌ No | ⚠️ Sometimes | ✅ **Standard** |

**CMake wins in every category!** ✨

---

## Why CMake is Industry Standard

### Real-World Projects Using CMake:

- **LLVM/Clang** - Compiler infrastructure
- **Qt** - Cross-platform GUI framework
- **OpenCV** - Computer vision library
- **Boost** - C++ libraries
- **MySQL** - Database
- **KDE** - Desktop environment
- **Unreal Engine** - Game engine
- **ROS** - Robot Operating System

**If you work with C++, you'll use CMake!**

---

## Best Practices

### 1. Always Use Out-of-Source Builds
```bash
mkdir build
cd build
cmake ..
```

**Never:**
```bash
cmake .  # ❌ In-source build (messy!)
```

### 2. Use .gitignore
Add `build/` to `.gitignore`:
```
build/
*.o
sensor_app
```

### 3. Separate Debug and Release
```bash
mkdir build-debug
mkdir build-release

cd build-debug
cmake -DCMAKE_BUILD_TYPE=Debug ..

cd build-release
cmake -DCMAKE_BUILD_TYPE=Release ..
```

### 4. Clean Rebuilds
When in doubt:
```bash
rm -rf build
mkdir build
cd build
cmake ..
cmake --build .
```

---

## What's Next?

### Learn More CMake

This workshop covers basics. CMake can also:
- Build libraries (static/shared)
- Find and link external packages
- Run tests (CTest)
- Install projects
- Package for distribution (CPack)
- Export compile commands for tools
- Generate documentation
- Much more!

### Resources

- [CMake Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/)
- [CMake Documentation](https://cmake.org/documentation/)
- [Modern CMake Guide](https://cliutils.gitlab.io/modern-cmake/)

---

## Congratulations! 🎉

You've completed the CMake workshop!

You now understand:
- ✅ Why build systems exist (pain of manual compilation)
- ✅ How Makefiles help (but have limitations)
- ✅ Why CMake is the modern solution
- ✅ How to use CMake for real projects

**Go forth and build amazing C++ projects!** 🚀

---

## Workshop Summary

**The Journey:**

1. **SimpleProject** - Easy (1 file)
2. **SimpleLibrary** - Getting annoying (2 files)
3. **BiggerLibrary** - Painful! (6 files, manual)
4. **BiggerLibraryMake** - Better (Makefile)
5. **BiggerLibraryCMake** - Best! (CMake) ← **You are here!**

**The Lesson:**

Manual compilation doesn't scale → Makefiles help → CMake perfects it!

---

Return to: `../../README.md` (Workshop overview)

---

**You're now a CMake developer!** 🎓
