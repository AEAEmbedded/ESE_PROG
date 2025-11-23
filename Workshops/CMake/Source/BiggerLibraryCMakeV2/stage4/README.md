# Stage 4: External Dependencies

**Automatic Dependency Management with FetchContent**

---

## Setup

```bash
cp ../../BiggerLibraryCMake/*.cpp .
cp -r ../../BiggerLibraryCMake/include .
```

**Note:** This stage requires internet connection for first build!

---

## What Changed

### New: FetchContent Module

```cmake
include(FetchContent)

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 10.1.1
)
FetchContent_MakeAvailable(fmt)
```

### Using the External Library

```cmake
target_link_libraries(utils_lib PUBLIC fmt::fmt)
```

Now `utils_lib` (and anything using it) can use the fmt library!

---

## Key Learnings

### 1. FetchContent Magic

When you run `cmake ..`, it will:
1. Download fmt library from GitHub
2. Build it from source
3. Make it available to your project
4. All automatically!

**No manual steps required!**

### 2. Version Pinning

```cmake
GIT_TAG 10.1.1
```

Ensures everyone builds with **exactly** the same version.
- Reproducible builds
- No "works on my machine" problems

### 3. Transitive Dependencies

```cmake
target_link_libraries(utils_lib PUBLIC fmt::fmt)  # utils_lib uses fmt
target_link_libraries(sensor_app PRIVATE utils_lib)  # app uses utils_lib
```

`sensor_app` automatically gets fmt because `utils_lib` declared it PUBLIC!

---

## Benefits

✅ **No Manual Downloads**
- No "please install fmt first"
- CMake handles it

✅ **Reproducible**
- Exact versions specified
- Same build everywhere

✅ **Cross-Platform**
- Works on Windows/Linux/macOS
- CMake adapts to each platform

✅ **Easy Updates**
- Change `GIT_TAG` to update version
- Rebuild - done!

---

## Build

```bash
mkdir build
cd build
cmake ..  # Watch CMake download and build fmt!
cmake --build .
./sensor_app
```

**First build is slower** (downloads and builds fmt).
**Subsequent builds are fast** (fmt is cached).

---

## What is fmt?

**fmt** is a modern C++ formatting library:

```cpp
// Old C++ way
std::cout << "Temperature: " << value << " degrees" << std::endl;

// fmt way (cleaner!)
fmt::print("Temperature: {} degrees\n", value);
```

More readable, type-safe, and faster than iostream!

---

## Exercise

### 1. See the Download

```bash
mkdir build
cd build
cmake ..
```

Watch the output - you'll see:
```
-- Fetching fmt...
-- Configuring fmt...
-- Building fmt...
```

CMake does all this automatically!

### 2. Explore the Cache

```bash
ls build/_deps/
```

See `fmt-src/` (source code) and `fmt-build/` (built library).

### 3. Add Another Library

Try adding spdlog (logging library):

```cmake
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.12.0
)
FetchContent_MakeAvailable(spdlog)

target_link_libraries(utils_lib PUBLIC spdlog::spdlog)
```

---

## Common Issues

### Problem: No Internet

FetchContent requires internet on **first build only**.

**Solution:**
- Build once with internet
- CMake caches the download
- Subsequent builds work offline

### Problem: Slow First Build

**Why:**
- CMake downloads source
- Builds from source
- Can take a few minutes

**Solution:**
- Be patient on first build
- Subsequent builds are fast (cached)

---

## Advanced: CMAKE_EXPORT_COMPILE_COMMANDS

```cmake
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
```

This generates `compile_commands.json`:
- Used by VS Code for IntelliSense
- Used by clang-tidy for static analysis
- Makes IDE integration better

---

## Next

Move to **Stage 5** for the professional version!
