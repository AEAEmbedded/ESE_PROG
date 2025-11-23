# Stage 5: Professional Version

**Production-Ready CMake Configuration**

---

## Setup

```bash
cp ../../BiggerLibraryCMake/*.cpp .
cp -r ../../BiggerLibraryCMake/include .
```

---

## What's New

This is a **professional-quality** CMake configuration with:

✅ Comprehensive project metadata
✅ Build options (user-configurable)
✅ Compiler-specific settings
✅ Installation rules
✅ Generator expressions
✅ Testing support
✅ Informative output

**This is how real projects do it!**

---

## Key Features

### 1. Full Project Metadata

```cmake
project(SensorApp
    VERSION 1.0.0
    DESCRIPTION "SHT45 Temperature/Humidity Sensor Monitor"
    HOMEPAGE_URL "https://github.com/han-university/sensor-app"
    LANGUAGES CXX
)
```

Provides complete project information.

### 2. Build Options

```cmake
option(BUILD_TESTS "Build unit tests" OFF)
option(USE_FMT "Use fmt library for formatting" ON)
option(ENABLE_INSTALL "Enable installation rules" ON)
```

Users can customize the build:

```bash
cmake -DBUILD_TESTS=ON ..       # Enable tests
cmake -DUSE_FMT=OFF ..          # Disable fmt
cmake -DENABLE_INSTALL=OFF ..   # Skip install rules
```

### 3. Compiler-Specific Warnings

```cmake
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(/W4)
endif()
```

Uses the right warnings for each compiler!

### 4. Generator Expressions

```cmake
target_include_directories(sht45
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
```

**$<BUILD_INTERFACE:...>** - Path when building
**$<INSTALL_INTERFACE:...>** - Path when installed

Smart path handling!

### 5. Installation Rules

```cmake
install(TARGETS sensor_app
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
```

Properly installs the project to system directories.

### 6. Library Properties

```cmake
set_target_properties(sht45 PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    OUTPUT_NAME "sensor_sht45"
)
```

Sets version info and proper naming.

### 7. Informative Output

```
=================================
SensorApp v1.0.0
=================================
Build type:        Release
C++ Standard:      C++17
Compiler:          AppleClang 15.0.0
Build tests:       OFF
Use fmt:           ON
Enable install:    ON
Install prefix:    /usr/local
=================================
```

Clear configuration summary!

---

## Build Variants

### Debug Build

```bash
mkdir build-debug
cd build-debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
```

- No optimization
- Debug symbols
- Good for debugging

### Release Build

```bash
mkdir build-release
cd build-release
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

- Full optimization
- No debug info
- Smaller, faster executable

### With Tests

```bash
cmake -DBUILD_TESTS=ON ..
cmake --build .
ctest  # Run tests
```

### Without External Dependencies

```bash
cmake -DUSE_FMT=OFF ..
cmake --build .
```

---

## Installation

### Install to System

```bash
mkdir build && cd build
cmake ..
cmake --build .
sudo cmake --install .
```

**Installs to:**
- `/usr/local/bin/sensor_app` (executable)
- `/usr/local/lib/libsensor_sht45.a` (library)
- `/usr/local/lib/libsensor_utils.a` (library)
- `/usr/local/include/SensorApp/*.hpp` (headers)

### Install to Custom Location

```bash
cmake -DCMAKE_INSTALL_PREFIX=$HOME/my_install ..
cmake --build .
cmake --install .
```

**Installs to:**
- `~/my_install/bin/sensor_app`
- `~/my_install/lib/libsensor_*.a`
- `~/my_install/include/SensorApp/*.hpp`

### Installation Components

```bash
# Install only the runtime (executable)
cmake --install . --component Runtime

# Install only development files (libraries + headers)
cmake --install . --component Development

# Install only documentation
cmake --install . --component Documentation
```

---

## Advanced Features Explained

### Generator Expressions

**What are they?**
Special syntax evaluated during build generation:

```cmake
$<BUILD_INTERFACE:path>   # Used when building project
$<INSTALL_INTERFACE:path> # Used when installed
```

**Why useful?**
Paths are different when building vs. when installed:
- Building: `${CMAKE_CURRENT_SOURCE_DIR}/include`
- Installed: `/usr/local/include/SensorApp`

Generator expressions handle both!

### GNUInstallDirs

```cmake
include(GNUInstallDirs)
```

Provides standard directory variables:
- `CMAKE_INSTALL_BINDIR` - Executables (`bin/`)
- `CMAKE_INSTALL_LIBDIR` - Libraries (`lib/`)
- `CMAKE_INSTALL_INCLUDEDIR` - Headers (`include/`)
- `CMAKE_INSTALL_DOCDIR` - Documentation (`share/doc/`)

Platform-aware (handles Linux/macOS/Windows differences).

### Target Properties

```cmake
set_target_properties(sht45 PROPERTIES
    VERSION 1.0.0
    SOVERSION 1
    OUTPUT_NAME "sensor_sht45"
)
```

- **VERSION** - Full version (1.0.0)
- **SOVERSION** - API version (1)
- **OUTPUT_NAME** - Override default name

Creates: `libsensor_sht45.so.1.0.0` (with symlinks)

---

## Options Reference

| Option | Default | Description |
|--------|---------|-------------|
| BUILD_TESTS | OFF | Build unit tests |
| USE_FMT | ON | Use fmt library |
| ENABLE_INSTALL | ON | Enable installation |
| CMAKE_BUILD_TYPE | Release | Debug or Release |
| CMAKE_INSTALL_PREFIX | /usr/local | Install location |

**Usage:**
```bash
cmake -D<OPTION>=<VALUE> ..
```

---

## Real-World Usage

This configuration is suitable for:

✅ **Open-source projects**
- Comprehensive metadata
- User options
- Installation support

✅ **Company projects**
- Professional structure
- Maintainable
- Scalable

✅ **Libraries**
- Proper versioning
- Install headers
- Reusable

✅ **Applications**
- Multiple build types
- Optional features
- System integration

---

## Comparison with Stage 1

### Stage 1 (Basic):
- 35 lines
- Works but basic
- No options
- No installation
- No metadata

### Stage 5 (Professional):
- 158 lines
- Production-ready
- User options
- Full installation
- Complete metadata
- Compiler-aware
- Platform-aware

**Worth the extra complexity!**

---

## Exercises

### 1. Build All Configurations

```bash
# Debug
mkdir build-debug && cd build-debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .

# Release
cd ..
mkdir build-release && cd build-release
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .

# Compare executable sizes:
ls -lh */sensor_app
```

Debug is bigger (has debug symbols)!

### 2. Try Options

```bash
# Without fmt
cmake -DUSE_FMT=OFF ..
cmake --build .

# With tests
cmake -DBUILD_TESTS=ON ..
cmake --build .
```

### 3. Install Locally

```bash
cmake -DCMAKE_INSTALL_PREFIX=$HOME/test_install ..
cmake --build .
cmake --install .
ls -R ~/test_install
```

See the installed structure!

### 4. Create a Package

```bash
# Install to staging area
cmake -DCMAKE_INSTALL_PREFIX=/tmp/sensor_package ..
cmake --build .
cmake --install .

# Create tarball
cd /tmp
tar czf sensor_package.tar.gz sensor_package/

# Now you can distribute this!
```

---

## What Professional Projects Add

Beyond this workshop, real projects often add:

- **CPack** - Package generation (.deb, .rpm, .msi)
- **CTest** - Comprehensive testing
- **Doxygen** - API documentation
- **Clang-tidy** - Static analysis
- **Coverage** - Code coverage reports
- **Sanitizers** - AddressSanitizer, etc.
- **CI/CD** - GitHub Actions, GitLab CI
- **Conan/vcpkg** - Package managers

---

## Best Practices Applied

✅ **Target-based** - Everything uses target commands
✅ **Visibility** - Proper PRIVATE/PUBLIC/INTERFACE
✅ **Modular** - Libraries + executable
✅ **Versioned** - Proper version handling
✅ **Installable** - Complete install rules
✅ **Configurable** - User options
✅ **Cross-platform** - Compiler detection
✅ **Documented** - Clear status messages
✅ **Professional** - Industry standard

---

## Congratulations! 🎉

You've completed all 5 stages!

You now know:
- ✅ Basic CMake (Stage 1)
- ✅ Modern target-based CMake (Stage 2)
- ✅ Library structure (Stage 3)
- ✅ External dependencies (Stage 4)
- ✅ **Professional CMake (Stage 5)**

**You're ready for real-world C++ projects!**

---

## Next Steps

1. **Use it!** - Apply to your own projects
2. **Study others** - Look at open-source CMake files
3. **Advanced topics** - Testing, packaging, CI/CD
4. **Share knowledge** - Help others learn CMake

---

Return to: `../README.md` (BiggerLibraryCMakeV2 overview)

Main workshop: `../../README.md`

---

**You're now a CMake professional!** 🚀
