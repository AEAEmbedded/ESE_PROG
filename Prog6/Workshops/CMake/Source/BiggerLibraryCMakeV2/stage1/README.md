# Stage 1: Basic CMake

**Starting Point - Functional but not Modern**

---

## Setup

Copy source files from BiggerLibraryCMake:

```bash
# From the stage1 directory:
cp ../../BiggerLibraryCMake/*.cpp .
cp -r ../../BiggerLibraryCMake/include .
```

Or manually create the files (same as BiggerLibraryCMake).

---

## What's Here

This is the **starting point** - the same basic CMake we learned in BiggerLibraryCMake.

**It works, but has issues:**
- ❌ `include_directories()` is global (affects all targets)
- ❌ `add_compile_options()` is global (affects all targets)
- ❌ Everything in one executable (no library reuse)

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

## Next

Move to **Stage 2** to see modern improvements!
