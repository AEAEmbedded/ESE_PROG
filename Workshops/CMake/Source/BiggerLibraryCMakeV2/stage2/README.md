# Stage 2: Modern CMake Basics

**Target-Specific Commands**

---

## Setup

```bash
cp ../../BiggerLibraryCMake/*.cpp .
cp -r ../../BiggerLibraryCMake/include .
```

---

## What Changed

### Before (Stage 1):
```cmake
include_directories(include)           # Global!
add_compile_options(-Wall -Wextra -O2) # Global!
```

### After (Stage 2):
```cmake
target_include_directories(sensor_app PRIVATE include)
target_compile_options(sensor_app PRIVATE -Wall -Wextra -O2)
```

---

## Key Learning: PRIVATE

The **PRIVATE** keyword means:
- These settings **only** affect `sensor_app`
- They don't leak to other targets
- Clear scope and intention

**Why this matters:**
If you add another executable or library later, it won't accidentally inherit these settings!

---

## Build

```bash
mkdir build
cd build
cmake ..
cmake --build .
./sensor_app
```

---

## Exercise

1. Add another executable (test_app)
2. Notice it **doesn't** get sensor_app's include paths
3. You must explicitly give it what it needs

This is **good** - explicit is better than implicit!

---

## Next

Move to **Stage 3** to learn library structure!
