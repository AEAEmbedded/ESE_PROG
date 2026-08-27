# Stage 6: Real-World STM32 Project

**Embedded CMake: From Workshop to Production**

---

## What This Is

This stage shows how **all the concepts from Stages 1-5 apply to real embedded development**. You'll see the same modern CMake practices, but adapted for cross-compilation, ARM Cortex-M microcontrollers, and professional firmware development.

**Target:** STM32F411 (ARM Cortex-M4) with SHT45 sensor

---

## 🎯 Key Concept

**Everything you learned applies to embedded!**

- ✅ Target-based commands (Stage 2)
- ✅ Libraries (Stage 3)
- ✅ External dependencies (Stage 4)
- ✅ Professional structure (Stage 5)
- **➕ Cross-compilation**
- **➕ Hardware-specific settings**
- **➕ Binary generation**

---

## What's New for Embedded

### 1. Toolchain File

**`arm-toolchain.cmake`** - Tells CMake we're cross-compiling:

```cmake
set(CMAKE_SYSTEM_NAME Generic)      # Not Linux/Windows
set(CMAKE_C_COMPILER arm-none-eabi-gcc)  # ARM compiler, not host gcc
```

**Why needed:**
- Your PC is x86/x64
- STM32 is ARM Cortex-M
- Need different compiler!

### 2. MCU-Specific Flags

```cmake
set(MCU_FLAGS
    -mcpu=cortex-m4        # Target this CPU
    -mthumb                # Use Thumb instructions
    -mfpu=fpv4-sp-d16     # Hardware FPU present
    -mfloat-abi=hard      # Use hardware floating point
)
```

**These tell the compiler exactly what hardware you have!**

### 3. Linker Script

```cmake
target_link_options(firmware.elf PRIVATE
    -T${MCU_LINKER_SCRIPT}  # Memory layout!
)
```

**Linker script defines:**
- Where is Flash memory? (code storage)
- Where is RAM? (data storage)
- How big are they?
- Where does the program start?

### 4. Binary Generation

```cmake
add_custom_command(TARGET firmware.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex firmware.elf firmware.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary firmware.elf firmware.bin
)
```

**Generates flashable files:**
- `.elf` - Debugging (symbols included)
- `.hex` - Intel HEX format (common)
- `.bin` - Raw binary (small)

---

## Project Structure

```
stage6/
├── CMakeLists.txt           # Main build configuration
├── arm-toolchain.cmake      # Cross-compilation setup
├── STM32F411RETx_FLASH.ld  # Memory layout
├── Drivers/                 # STM32 HAL
│   ├── STM32F4xx_HAL_Driver/
│   └── CMSIS/
├── Core/                    # System files
│   ├── Inc/
│   │   ├── stm32f4xx_hal_conf.h
│   │   └── stm32f4xx_it.h
│   └── Src/
│       ├── system_stm32f4xx.c
│       └── stm32f4xx_it.c
├── src/                     # Your application code
│   ├── main.cpp
│   ├── sht45.cpp           # Same library from workshop!
│   └── i2c_wrapper.cpp     # STM32 HAL wrapper
├── include/
│   ├── sht45.hpp           # Same as workshop!
│   └── i2c_wrapper.hpp
└── startup_stm32f411xe.s   # Assembly startup code
```

---

## How It Works

### 1. Toolchain Setup

```cmake
# MUST be before project()!
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/arm-toolchain.cmake)
```

CMake reads the toolchain file and knows:
- Use ARM compiler (not PC compiler)
- Target is embedded (not PC)
- Don't try to run tests (can't execute ARM code on PC!)

### 2. HAL Library

```cmake
add_library(stm32_hal STATIC ...)
target_include_directories(stm32_hal PUBLIC ...)
target_compile_options(stm32_hal PRIVATE ${MCU_FLAGS})
```

**Same pattern as Stage 3!** HAL is just another library.

### 3. Your Sensor Library

```cmake
add_library(sht45 STATIC ...)
target_link_libraries(sht45 PUBLIC stm32_hal)
```

**Same as workshop!** Your library links to HAL (just like workshop linked to fmt).

### 4. Main Firmware

```cmake
add_executable(${PROJECT_NAME}.elf ...)
target_link_libraries(${PROJECT_NAME}.elf PRIVATE sht45 stm32_hal)
```

**Same pattern!** Executable links to your libraries.

### 5. Post-Build Magic

```cmake
add_custom_command(TARGET firmware.elf POST_BUILD
    COMMAND ${ARM_OBJCOPY} -O ihex firmware.elf firmware.hex
)
```

**After building, generate flashable files automatically!**

---

## Prerequisites

### Required Tools

**ARM GCC Toolchain:**
```bash
# macOS
brew install --cask gcc-arm-embedded

# Ubuntu/Debian
sudo apt-get install gcc-arm-none-eabi

# Windows
# Download from: https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm
```

**OpenOCD (for flashing):**
```bash
# macOS
brew install openocd

# Ubuntu/Debian
sudo apt-get install openocd
```

**Verify installation:**
```bash
arm-none-eabi-gcc --version
arm-none-eabi-g++ --version
openocd --version
```

---

## Building

### Configure

```bash
mkdir build
cd build
cmake ..
```

**CMake will:**
1. Read toolchain file
2. Find ARM GCC compiler
3. Configure for STM32
4. Generate build system

### Build

```bash
cmake --build .
```

**Output files:**
```
build/
├── STM32_SHT45.elf    # Debug version (with symbols)
├── STM32_SHT45.hex    # For programming
├── STM32_SHT45.bin    # Raw binary
└── STM32_SHT45.map    # Memory map
```

### Check Size

```bash
arm-none-eabi-size STM32_SHT45.elf
```

**Output:**
```
   text    data     bss     dec     hex filename
  12345     100    2048   14493    3895 STM32_SHT45.elf
```

- **text** - Code (Flash)
- **data** - Initialized data (Flash → RAM)
- **bss** - Uninitialized data (RAM)

### Flash to Hardware

```bash
make flash
```

Or manually:
```bash
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg \
    -c "program STM32_SHT45.elf verify reset exit"
```

---

## Key Embedded Concepts

### Cross-Compilation

**Host:** Your PC (x86_64, Linux/macOS/Windows)
**Target:** STM32 (ARM Cortex-M4, bare metal)

```
┌─────────────┐                 ┌─────────────┐
│   Your PC   │   compiles for  │   STM32     │
│   (x86_64)  │  ────────────>  │   (ARM)     │
└─────────────┘                 └─────────────┘
    Host                           Target
```

Can't run ARM code on PC!
Can't use PC libraries on STM32!
Need special compiler!

### Memory Layout

**STM32F411 has:**
- Flash: 512 KB (code storage, non-volatile)
- RAM: 128 KB (data storage, volatile)

**Linker script defines:**
```
MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
    RAM (rwx)   : ORIGIN = 0x20000000, LENGTH = 128K
}
```

**Your code must fit!**

### Startup Sequence

1. **Power on**
2. **Hardware reset**
3. **startup_stm32f411xe.s** (Assembly)
   - Set stack pointer
   - Copy .data from Flash to RAM
   - Zero .bss in RAM
   - Call system init
4. **system_stm32f4xx.c** (C)
   - Configure clocks (168 MHz)
   - Enable FPU
5. **main()** (Your code!)

**All handled by CMake build!**

---

## Comparing with Workshop

### Same Concepts, Different Target!

| Feature | Workshop (PC) | Embedded (STM32) |
|---------|--------------|-------------------|
| **Compiler** | g++ | arm-none-eabi-g++ |
| **Target** | x86_64 | ARM Cortex-M4 |
| **OS** | Linux/macOS | Bare metal (none!) |
| **Output** | executable | .elf + .hex + .bin |
| **Libraries** | std::cout, fmt | HAL, CMSIS |
| **Run** | `./sensor_app` | Flash to hardware |

### Same CMake Patterns!

**Library creation:**
```cmake
# Workshop
add_library(utils STATIC logger.cpp config.cpp)

# Embedded
add_library(stm32_hal STATIC hal_i2c.c hal_gpio.c)
```

**Linking:**
```cmake
# Workshop
target_link_libraries(sensor_app PRIVATE utils)

# Embedded
target_link_libraries(firmware.elf PRIVATE sht45)
```

**Include paths:**
```cmake
# Workshop
target_include_directories(utils PUBLIC include)

# Embedded
target_include_directories(stm32_hal PUBLIC Drivers/Inc)
```

**The patterns are the same!** 🎯

---

## Professional Embedded Projects

Real projects add:

### 1. Multiple Targets

```cmake
option(TARGET_BOARD "Target board" "STM32F411-DISCO")

if(TARGET_BOARD STREQUAL "STM32F411-DISCO")
    set(MCU_MODEL STM32F411xE)
    set(MCU_LINKER_SCRIPT STM32F411RETx_FLASH.ld)
elseif(TARGET_BOARD STREQUAL "NUCLEO-F411RE")
    # Different configuration
endif()
```

### 2. Build Configurations

```cmake
# Debug: No optimization, debug symbols
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Release: Full optimization, no debug
cmake -DCMAKE_BUILD_TYPE=Release ..

# MinSizeRel: Optimize for size
cmake -DCMAKE_BUILD_TYPE=MinSizeRel ..
```

### 3. Unit Testing

```cmake
if(BUILD_TESTS)
    # Build tests for PC (not STM32)
    set(CMAKE_TOOLCHAIN_FILE "")  # Use host compiler
    add_subdirectory(tests)
endif()
```

Tests run on PC, firmware runs on STM32!

### 4. Continuous Integration

```yaml
# .github/workflows/build.yml
- name: Build firmware
  run: |
    cmake -B build
    cmake --build build

- name: Check size
  run: arm-none-eabi-size build/firmware.elf
```

---

## Common Issues

### Problem: arm-none-eabi-gcc not found

**Solution:**
```bash
# Install ARM GCC toolchain
brew install gcc-arm-embedded  # macOS
sudo apt install gcc-arm-none-eabi  # Ubuntu

# Or add to PATH
export PATH="/path/to/arm-gcc/bin:$PATH"
```

### Problem: Code doesn't fit in Flash

**Error:**
```
region `FLASH' overflowed by 1234 bytes
```

**Solutions:**
- Increase optimization (`-Os` for size)
- Remove unused code
- Use smaller printf implementation
- Upgrade to larger STM32 model

### Problem: Hard fault at runtime

**Debug:**
```bash
# Start GDB server
make debug

# In another terminal
arm-none-eabi-gdb build/firmware.elf
(gdb) target remote :3333
(gdb) monitor reset halt
(gdb) continue
```

---

## Exercises

### 1. Analyze Memory Usage

```bash
arm-none-eabi-size -A STM32_SHT45.elf
```

Understand what's using Flash vs RAM.

### 2. Change Optimization

Edit CMakeLists.txt:
```cmake
# From: -O2 (optimize for speed)
# To:   -Os (optimize for size)
```

Rebuild and compare sizes!

### 3. Add UART Logging

```cmake
# Add HAL UART
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
```

Replace logger.cpp with UART output!

### 4. Multi-Config Build

```bash
# Create both debug and release
mkdir build-debug build-release

cd build-debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

cd ../build-release
cmake -DCMAKE_BUILD_TYPE=Release ..
make

# Compare sizes
arm-none-eabi-size */STM32_SHT45.elf
```

---

## Summary

### What You Learned

✅ **Cross-compilation** - Building for different architecture
✅ **Toolchain files** - Configuring for embedded targets
✅ **MCU-specific settings** - Cortex-M flags and options
✅ **HAL integration** - Using vendor libraries
✅ **Linker scripts** - Memory layout definition
✅ **Binary generation** - Creating flashable files
✅ **Post-build commands** - Automation
✅ **Professional embedded** - Real-world practices

### The Big Picture

**Workshop concepts → Embedded reality:**

1. **Stages 1-2:** Basic CMake
   → **Stage 6:** Cross-compilation basics

2. **Stage 3:** Libraries
   → **Stage 6:** HAL as library, your code as library

3. **Stage 4:** External dependencies
   → **Stage 6:** CMSIS, HAL drivers

4. **Stage 5:** Professional structure
   → **Stage 6:** Production firmware

**Same modern CMake, different target!** 🎯

---

## Resources

### ARM GCC
- [ARM GCC Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm)

### STM32
- [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html) - Code generator
- [STM32 HAL Documentation](https://www.st.com/en/embedded-software/stm32cube-mcu-mpu-packages.html)

### Embedded CMake
- [CMake for Embedded](https://cmake.org/cmake/help/book/mastering-cmake/chapter/Cross%20Compiling%20With%20CMake.html)
- [Modern CMake for ARM](https://interrupt.memfault.com/blog/a-modern-c-dev-environment)

---

## Congratulations! 🎉

You've completed **all 6 stages** of the modern CMake workshop!

You can now:
- ✅ Write modern CMake for PC applications
- ✅ Write modern CMake for embedded systems
- ✅ Apply professional practices
- ✅ Understand cross-compilation
- ✅ Build real firmware

**You're ready for professional C++ and embedded development!** 🚀

---

Return to: `../README.md` (BiggerLibraryCMakeV2 overview)

Main workshop: `../../README.md`
