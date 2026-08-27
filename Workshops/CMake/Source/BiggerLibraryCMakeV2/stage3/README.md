# Stage 3: Library Structure

**Modular Architecture with Reusable Libraries**

---

## Setup

```bash
cp ../../BiggerLibraryCMake/*.cpp .
cp -r ../../BiggerLibraryCMake/include .
```

---

## What Changed

### Before (Stage 2):
```cmake
add_executable(sensor_app main.cpp sht45.cpp i2c.cpp logger.cpp config.cpp filter.cpp)
```

Everything in one big executable - no reuse possible.

### After (Stage 3):
```cmake
# Separate libraries
add_library(sht45_lib STATIC sht45.cpp i2c.cpp)
add_library(utils_lib STATIC logger.cpp config.cpp filter.cpp)

# Small executable
add_executable(sensor_app main.cpp)

# Link them together
target_link_libraries(sensor_app PRIVATE sht45_lib utils_lib)
```

Now we have reusable libraries!

---

## Key Learnings

### 1. Static Libraries

```cmake
add_library(sht45_lib STATIC sht45.cpp i2c.cpp)
```

Creates `libsht45_lib.a` (Unix) or `sht45_lib.lib` (Windows)
- Compiled once
- Linked into executables
- Reusable across projects

### 2. PUBLIC vs PRIVATE

```cmake
target_include_directories(sht45_lib PUBLIC include)
```

**PUBLIC** means:
- `sht45_lib` uses `include/`
- **AND** anything linking to `sht45_lib` gets `include/` too!

That's why `sensor_app` doesn't need to specify include directories!

```cmake
target_link_libraries(sensor_app PRIVATE sht45_lib)
# sensor_app automatically gets include/ from sht45_lib!
```

### 3. Dependency Chain

```
sensor_app
    ↓ (links to)
sht45_lib + utils_lib
    ↓ (provides)
include/ directories
```

CMake handles the dependency chain automatically!

---

## Benefits

✅ **Reusability**
- Use `sht45_lib` in another project
- Use `utils_lib` independently

✅ **Faster Builds**
- Change `logger.cpp`? Only rebuild `utils_lib`
- Main app and `sht45_lib` don't rebuild

✅ **Clear Organization**
- Hardware layer: `sht45_lib`
- Utilities layer: `utils_lib`
- Application layer: `sensor_app`

✅ **Testing**
- Can test libraries independently
- Mock dependencies easily

---

## Build

```bash
mkdir build
cd build
cmake ..
cmake --build .
ls  # See libsht45_lib.a and libutils_lib.a!
./sensor_app
```

---

## Exercise

### 1. Understand PUBLIC

Change `sht45_lib`'s include to PRIVATE:

```cmake
target_include_directories(sht45_lib PRIVATE include)  # Changed to PRIVATE
```

Build - it fails! Why?
- `sensor_app` can't see headers anymore
- Because PRIVATE means "only for this target"

Change it back to PUBLIC to fix.

### 2. Create Another Executable

```cmake
add_executable(test_app test.cpp)
target_link_libraries(test_app PRIVATE utils_lib)
```

Now `test_app` can use the logger!

---

## Next

Move to **Stage 4** to add external dependencies!
