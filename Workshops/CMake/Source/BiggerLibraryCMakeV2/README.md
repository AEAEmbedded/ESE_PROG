# BiggerLibraryCMakeV2 - Modern CMake Workshop

**HAN University of Applied Sciences**
**Advanced CMake: From Basic to Professional**

---

## What This Is

This is an **advanced CMake workshop** showing the progression from basic CMake to professional, production-quality CMake configuration. Each stage improves on the previous one, teaching modern CMake best practices.

---

## 📦 Repository

**GitHub:** [https://github.com/AEAEmbedded/ESE_PROG](https://github.com/AEAEmbedded/ESE_PROG)

**Direct Link to This Workshop:**
```
https://github.com/AEAEmbedded/ESE_PROG/tree/main/Workshops/CMake/Source/BiggerLibraryCMakeV2
```

### Getting the Code

**Clone the entire repository:**
```bash
git clone https://github.com/AEAEmbedded/ESE_PROG.git
cd ESE_PROG/Workshops/CMake/Source/BiggerLibraryCMakeV2
```

**Or download just this workshop:**
```bash
# Download as ZIP from GitHub
# Navigate to: https://github.com/AEAEmbedded/ESE_PROG
# Click "Code" → "Download ZIP"
```

**Each stage is self-contained** with all source files, so you can work through them independently!

---

## Workshop Structure

```
BiggerLibraryCMakeV2/
├── README.md              # This file
├── stage1/               # Basic CMake (starting point)
├── stage2/               # Modern CMake basics
├── stage3/               # Library structure
├── stage4/               # External dependencies
├── stage5/               # Professional version
└── stage6/               # Real-world STM32 embedded project
```

---

## The 6 Stages

### Stage 1: Basic CMake (Starting Point)
**What you already know from BiggerLibraryCMake**

- Old-style `include_directories()` (global scope)
- Global `add_compile_options()`
- Everything in one executable
- Works, but not modern or scalable

**File:** `stage1/CMakeLists.txt` (35 lines)

**Teaching Point:** "This works, but we can do better!"

---

### Stage 2: Modern CMake Basics
**Target-specific properties**

**Key Improvements:**
- ✅ `target_include_directories()` instead of `include_directories()`
- ✅ `target_compile_options()` instead of global `add_compile_options()`
- ✅ **PRIVATE** visibility - settings only affect this target

**File:** `stage2/CMakeLists.txt`

**Teaching Point:** "See PRIVATE? This means only sensor_app uses these settings, not everything in the project!"

**Why this matters:**
```cmake
# OLD WAY (global - affects everything)
include_directories(include)
add_compile_options(-Wall)

# NEW WAY (target-specific - clean separation)
target_include_directories(sensor_app PRIVATE include)
target_compile_options(sensor_app PRIVATE -Wall)
```

---

### Stage 3: Library Structure
**Separate compilation units**

**Key Improvements:**
- ✅ Create reusable libraries with `add_library()`
- ✅ Link libraries with `target_link_libraries()`
- ✅ **PUBLIC** vs **PRIVATE** dependencies
- ✅ Modular architecture

**File:** `stage3/CMakeLists.txt`

**Libraries created:**
- `sht45_lib` - Sensor hardware (sht45.cpp, i2c.cpp)
- `utils_lib` - Utilities (logger.cpp, config.cpp, filter.cpp)

**Teaching Point:** "Now we have reusable libraries! You could use sht45_lib in another project!"

**Why this matters:**
- Libraries can be reused in multiple executables
- Better organization and separation of concerns
- Faster incremental builds (only rebuild changed libraries)
- Can create static (.a) or shared (.so/.dll) libraries

---

### Stage 4: External Dependencies
**Modern dependency management**

**Key Improvements:**
- ✅ `FetchContent` for external libraries
- ✅ No manual download/install required
- ✅ Automatic dependency resolution
- ✅ `CMAKE_EXPORT_COMPILE_COMMANDS` for IDE integration

**File:** `stage4/CMakeLists.txt`

**External library added:**
- **fmt** - Modern C++ formatting library (better than printf/iostream)

**Teaching Point:** "CMake downloads and builds dependencies automatically!"

**Why this matters:**
- No "dependency hell" - CMake handles it
- Reproducible builds - same code, same dependencies
- Cross-platform - works on all systems
- Version pinning - exactly which version you want

---

### Stage 5: Professional Version
**Production-ready CMake**

**Key Improvements:**
- ✅ Comprehensive project metadata
- ✅ Build type handling (Debug/Release)
- ✅ Compiler-specific warnings
- ✅ Build options (toggleable features)
- ✅ Installation rules
- ✅ Generator expressions for advanced control
- ✅ Testing support
- ✅ Informative status messages

**File:** `stage5/CMakeLists.txt`

**Features added:**
- Installation rules (`install()`)
- Options (`option()` for BUILD_TESTS, USE_FMT)
- Generator expressions (`$<BUILD_INTERFACE:...>`)
- Conditional compilation (MSVC vs GCC/Clang warnings)
- Comprehensive status output

**Teaching Point:** "This is how professional projects configure CMake!"

**Why this matters:**
- Used in industry and open-source projects
- Supports complex build scenarios
- Maintainable and extensible
- Follows CMake best practices

---

### Stage 6: Real-World STM32 Embedded Project
**Everything you learned applies to embedded!**

**Key Features:**
- ✅ All concepts from Stages 1-5 applied to embedded
- ✅ Cross-compilation for ARM Cortex-M4
- ✅ Toolchain file configuration
- ✅ STM32 HAL integration
- ✅ MCU-specific compiler flags
- ✅ Linker script integration
- ✅ Binary generation (.hex, .bin files)
- ✅ Flash and debug targets

**File:** `stage6/CMakeLists.txt`

**Target:** STM32F411 (ARM Cortex-M4) with SHT45 sensor

**Teaching Point:** "The same modern CMake patterns work for embedded! Cross-compilation, libraries, dependencies—it all applies!"

**Why this matters:**
- Shows workshop concepts in production embedded firmware
- Demonstrates cross-platform nature of CMake
- Real-world professional embedded development
- Same patterns: libraries, linking, target-based commands
- Just different target architecture (ARM instead of x86)

**What's different for embedded:**
```cmake
# Toolchain file (before project()!)
set(CMAKE_TOOLCHAIN_FILE arm-toolchain.cmake)

# MCU-specific flags
set(MCU_FLAGS -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16)

# Linker script (defines memory layout)
target_link_options(firmware.elf PRIVATE -T${LINKER_SCRIPT})

# Generate flashable binaries
add_custom_command(TARGET firmware.elf POST_BUILD
    COMMAND ${ARM_OBJCOPY} -O ihex firmware.elf firmware.hex
)
```

**See the full README:** `stage6/README.md` for complete embedded CMake guide!

---

## How to Use This Workshop

### Sequential Learning (Recommended)

Work through each stage in order:

```bash
# Stage 1
cd stage1
mkdir build && cd build
cmake ..
cmake --build .
./sensor_app

# Stage 2
cd ../../stage2
mkdir build && cd build
cmake ..
cmake --build .
./sensor_app

# Continue through stage3, stage4, stage5, stage6...
```

**Note for Stage 6:** Requires ARM GCC toolchain for embedded development. See `stage6/README.md` for prerequisites.

### Compare Stages

Open two CMakeLists.txt files side-by-side to see what changed:

```bash
# Compare Stage 1 vs Stage 2
diff stage1/CMakeLists.txt stage2/CMakeLists.txt
```

---

## Key Concepts

### 1. Target-Based Commands

**Old CMake (pre-3.0):**
```cmake
include_directories(include)           # Global - affects all targets
add_compile_options(-Wall)            # Global - affects all targets
```

**Modern CMake (3.0+):**
```cmake
target_include_directories(myapp PRIVATE include)  # Only affects myapp
target_compile_options(myapp PRIVATE -Wall)        # Only affects myapp
```

**Benefits:**
- Clear dependencies
- No accidental pollution
- Easier to understand
- Better for large projects

---

### 2. Visibility Specifiers

**PRIVATE** - Only this target uses it
```cmake
target_include_directories(mylib PRIVATE src/)
# Only mylib sees src/ headers
```

**PUBLIC** - This target and anything linking to it uses it
```cmake
target_include_directories(mylib PUBLIC include/)
# mylib sees include/, AND anything that links to mylib sees it too
```

**INTERFACE** - Only targets linking to this use it
```cmake
target_include_directories(mylib INTERFACE include/)
# mylib doesn't see it, but targets linking to mylib do
```

**Example:**
```cmake
add_library(sht45_lib sht45.cpp)
target_include_directories(sht45_lib PUBLIC include)  # PUBLIC!

add_executable(app main.cpp)
target_link_libraries(app PRIVATE sht45_lib)
# app automatically gets include/ because sht45_lib declared it PUBLIC
```

---

### 3. Libraries vs Executables

**Static Library (.a on Unix, .lib on Windows):**
```cmake
add_library(mylib STATIC source.cpp)
```
- Compiled into executable at link time
- Larger executable, but self-contained

**Shared Library (.so on Unix, .dll on Windows):**
```cmake
add_library(mylib SHARED source.cpp)
```
- Loaded at runtime
- Smaller executable, shared between programs

**Executable:**
```cmake
add_executable(myapp main.cpp)
```

---

### 4. Modern Dependency Management

**Old way (manual):**
1. Download library
2. Build it
3. Install it
4. Tell CMake where to find it
5. Hope it works on other machines

**New way (FetchContent):**
```cmake
include(FetchContent)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 10.1.1
)
FetchContent_MakeAvailable(fmt)

target_link_libraries(myapp PRIVATE fmt::fmt)
```

**Benefits:**
- Automatic download
- Correct version every time
- Works on all platforms
- Part of build configuration

---

## Stage-by-Stage Comparison

| Feature | Stage 1 | Stage 2 | Stage 3 | Stage 4 | Stage 5 | Stage 6 |
|---------|---------|---------|---------|---------|---------|---------|
| Lines of code | 35 | 32 | 42 | 58 | 95 | 158 |
| Target-specific commands | ❌ | ✅ | ✅ | ✅ | ✅ | ✅ |
| Libraries | ❌ | ❌ | ✅ | ✅ | ✅ | ✅ |
| External deps | ❌ | ❌ | ❌ | ✅ | ✅ | ✅ (HAL) |
| Installation | ❌ | ❌ | ❌ | ❌ | ✅ | ⚠️ |
| Options | ❌ | ❌ | ❌ | ❌ | ✅ | ⚠️ |
| Build types | ⚠️ | ⚠️ | ⚠️ | ⚠️ | ✅ | ✅ |
| Cross-compilation | ❌ | ❌ | ❌ | ❌ | ❌ | ✅ |
| Toolchain file | ❌ | ❌ | ❌ | ❌ | ❌ | ✅ |
| Binary generation | ❌ | ❌ | ❌ | ❌ | ❌ | ✅ |
| MCU-specific | ❌ | ❌ | ❌ | ❌ | ❌ | ✅ |
| Complexity | Simple | Simple | Medium | Medium | Advanced | Production |
| Professional? | No | Better | Good | Very Good | Yes! | Embedded! |

---

## Exercises

### Exercise 1: Understand Visibility

**In Stage 3:**

1. Look at how `sht45_lib` uses **PUBLIC** for include directories
2. Notice how `sensor_app` doesn't need to specify include directories
3. Change PUBLIC to PRIVATE - see the build fail!
4. Understand why: app can't see headers anymore

### Exercise 2: Add Your Own Library

**In Stage 3:**

1. Create `math_lib` with a simple calculation function
2. Add it as a new library
3. Link it to `sensor_app`
4. Use it in main.cpp

### Exercise 3: Add External Dependency

**In Stage 4:**

1. The project uses `fmt` library
2. Try removing FetchContent - see build fail
3. Add it back
4. Notice CMake downloads and builds it automatically

### Exercise 4: Configure Options

**In Stage 5:**

1. Build with `cmake -DBUILD_TESTS=ON ..`
2. Build with `cmake -DUSE_FMT=OFF ..`
3. See how options change the build
4. Add your own option

### Exercise 5: Install the Project

**In Stage 5:**

```bash
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=~/my_install ..
cmake --build .
cmake --install .
ls ~/my_install
```

See the installed files!

---

## Common Questions

### Q: Why use target-specific commands?

**A:** Prevents accidental dependencies and makes code clearer.

**Example problem without them:**
```cmake
include_directories(lib1/include)  # Global
include_directories(lib2/include)  # Global

add_library(lib1 ...)  # Sees both lib1 and lib2 includes!
add_library(lib2 ...)  # Sees both lib1 and lib2 includes!
```

Now lib1 might accidentally use lib2 headers!

**With target-specific:**
```cmake
add_library(lib1 ...)
target_include_directories(lib1 PRIVATE lib1/include)  # Only lib1

add_library(lib2 ...)
target_include_directories(lib2 PRIVATE lib2/include)  # Only lib2
```

Clear separation!

### Q: When to use PRIVATE vs PUBLIC?

**Rule of thumb:**
- **PRIVATE** - Implementation details (internal headers, flags)
- **PUBLIC** - Part of the interface (public headers that users need)
- **INTERFACE** - Header-only libraries

**Example:**
```cmake
add_library(mylib mylib.cpp internal.cpp)

# Private: only mylib needs this
target_include_directories(mylib PRIVATE src/internal)

# Public: anyone using mylib needs this
target_include_directories(mylib PUBLIC include)
```

### Q: Why separate libraries?

**Benefits:**
1. **Reusability** - Use in multiple projects
2. **Faster builds** - Only rebuild what changed
3. **Clear dependencies** - See what depends on what
4. **Testing** - Test libraries independently

### Q: Should I always use FetchContent?

**When to use:**
- ✅ Small header-only libraries
- ✅ Build from source is fast
- ✅ Need specific version
- ✅ Library has good CMake support

**When not to use:**
- ❌ Large libraries (use system packages instead)
- ❌ Build time very slow
- ❌ Need system integration

---

## Best Practices Summary

### Do ✅

1. **Use target-specific commands**
   ```cmake
   target_include_directories(mytarget PRIVATE include)
   ```

2. **Use visibility specifiers correctly**
   ```cmake
   target_link_libraries(app PRIVATE mylib)
   ```

3. **Organize into libraries**
   ```cmake
   add_library(mylib source.cpp)
   add_executable(app main.cpp)
   target_link_libraries(app PRIVATE mylib)
   ```

4. **Use modern CMake features**
   ```cmake
   cmake_minimum_required(VERSION 3.20)
   ```

5. **Set C++ standard portably**
   ```cmake
   set(CMAKE_CXX_STANDARD 17)
   set(CMAKE_CXX_STANDARD_REQUIRED ON)
   ```

### Don't ❌

1. **Don't use global commands**
   ```cmake
   include_directories(...)  # ❌ Old style
   add_compile_options(...)  # ❌ Affects everything
   ```

2. **Don't forget visibility**
   ```cmake
   target_link_libraries(app mylib)  # ❌ Should specify PRIVATE/PUBLIC
   ```

3. **Don't put everything in one target**
   ```cmake
   add_executable(app source1.cpp source2.cpp source3.cpp ...)  # ❌ No reuse
   ```

4. **Don't require old CMake**
   ```cmake
   cmake_minimum_required(VERSION 2.8)  # ❌ Too old
   ```

---

## What You'll Learn

### After Stage 1-2:
- Modern target-based CMake
- PRIVATE vs PUBLIC vs INTERFACE

### After Stage 3:
- Creating and using libraries
- Proper project organization
- Linking libraries together

### After Stage 4:
- External dependency management
- FetchContent module
- Automatic downloads

### After Stage 5:
- Professional CMake structure
- Installation rules
- Build options
- Conditional compilation
- Cross-platform support

### After Stage 6:
- Cross-compilation for embedded systems
- Toolchain file configuration
- ARM Cortex-M specific settings
- Linker script integration
- Binary generation for microcontrollers
- Real-world embedded firmware development
- How workshop concepts apply to production embedded code

---

## Next Steps

After completing all 6 stages:

1. **Apply to your projects** - Use modern CMake in real work
2. **Read documentation** - [CMake docs](https://cmake.org/documentation/)
3. **Study real projects** - Look at open-source CMake configs
4. **Advanced topics**:
   - Testing with CTest
   - Packaging with CPack
   - Custom commands and targets
   - Find modules
   - Config files for libraries

---

## Resources

### Official Documentation
- [CMake Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/)
- [CMake Commands](https://cmake.org/cmake/help/latest/manual/cmake-commands.7.html)
- [Modern CMake Guide](https://cliutils.gitlab.io/modern-cmake/)

### Example Projects
- [fmt library](https://github.com/fmtlib/fmt) - Excellent CMake
- [spdlog](https://github.com/gabime/spdlog) - Logging library
- [Catch2](https://github.com/catchorg/Catch2) - Testing framework

---

## Summary

**The Journey:**

1. **Stage 1** - Basic but works
2. **Stage 2** - Modern target-based approach
3. **Stage 3** - Proper library structure
4. **Stage 4** - External dependencies
5. **Stage 5** - Professional quality

**The Lesson:**

Start simple, learn the fundamentals, then add complexity as needed. Modern CMake is powerful, clean, and professional!

---

**You're becoming a CMake expert!** 🚀

---

Return to: `../README.md` (Main workshop)
