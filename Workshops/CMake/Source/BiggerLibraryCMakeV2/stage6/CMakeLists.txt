# CMakeLists.txt - Stage 6: Real-World STM32 Project
# CMake Workshop - HAN University of Applied Sciences
# Johan Korten - v1.0 2025
#
# STAGE 6: Real Embedded Example (STM32 ARM Cortex-M)
# Shows how workshop concepts apply to actual embedded development

cmake_minimum_required(VERSION 3.20)

# CRITICAL: Toolchain file MUST be set before project()
# This tells CMake we're cross-compiling for ARM, not the host system
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/arm-toolchain.cmake)

# Project definition
project(STM32_SHT45
    VERSION 1.0.0
    DESCRIPTION "SHT45 Sensor on STM32F4"
    HOMEPAGE_URL "https://github.com/AEAEmbedded/ESE_PROG"
    LANGUAGES C CXX ASM  # ASM needed for startup code!
)

#
# MCU-Specific Settings
#
set(MCU_FAMILY STM32F4xx)
set(MCU_MODEL STM32F411xE)
set(MCU_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/STM32F411RETx_FLASH.ld)

# ARM Cortex-M4 compiler flags
set(MCU_FLAGS
    -mcpu=cortex-m4        # Target CPU
    -mthumb                # Thumb instruction set
    -mfpu=fpv4-sp-d16     # Hardware FPU
    -mfloat-abi=hard      # Use hardware FP
)

# Common compile definitions
add_compile_definitions(
    ${MCU_MODEL}
    USE_HAL_DRIVER
    ARM_MATH_CM4
)

#
# STM32 HAL Library (Hardware Abstraction Layer)
#
add_library(stm32_hal STATIC
    # HAL Core
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c

    # I2C for sensor communication
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c

    # DMA for efficient transfers (optional)
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c

    # Add more HAL modules as needed:
    # Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
    # Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c
)

target_include_directories(stm32_hal PUBLIC
    Drivers/STM32F4xx_HAL_Driver/Inc
    Drivers/CMSIS/Device/ST/${MCU_FAMILY}/Include
    Drivers/CMSIS/Include
    Core/Inc  # User configuration headers
)

target_compile_options(stm32_hal PRIVATE ${MCU_FLAGS} -Wall -O2)

#
# Sensor Library (Same concepts from workshop!)
#
add_library(sht45 STATIC
    src/sht45.cpp
    src/i2c_wrapper.cpp  # Wraps STM32 HAL I2C functions
)

target_include_directories(sht45 PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# Link to HAL (sensor needs I2C)
target_link_libraries(sht45 PUBLIC stm32_hal)
target_compile_options(sht45 PRIVATE ${MCU_FLAGS} -Wall -Wextra)

#
# System Files
#
add_library(system OBJECT
    Core/Src/system_stm32f4xx.c     # System clock configuration
    Core/Src/stm32f4xx_it.c         # Interrupt handlers
    startup_stm32f411xe.s            # Assembly startup code
)

target_include_directories(system PUBLIC Core/Inc)
target_compile_options(system PRIVATE ${MCU_FLAGS})

#
# Main Firmware Executable
#
add_executable(${PROJECT_NAME}.elf
    src/main.cpp
    $<TARGET_OBJECTS:system>  # Include system files
)

# Link all libraries
target_link_libraries(${PROJECT_NAME}.elf
    PRIVATE
        sht45        # Our sensor library
        stm32_hal    # STM32 HAL
)

target_include_directories(${PROJECT_NAME}.elf PRIVATE
    Core/Inc
    include
)

target_compile_options(${PROJECT_NAME}.elf PRIVATE
    ${MCU_FLAGS}
    -Wall
    -Wextra
    -fdata-sections    # Enable garbage collection
    -ffunction-sections
)

#
# Linker Settings (Critical for embedded!)
#
target_link_options(${PROJECT_NAME}.elf PRIVATE
    -T${MCU_LINKER_SCRIPT}           # Memory layout
    -Wl,-Map=${PROJECT_NAME}.map     # Generate map file
    -Wl,--gc-sections                # Remove unused code
    -Wl,--print-memory-usage         # Show memory usage
    ${MCU_FLAGS}
    --specs=nano.specs               # Use smaller newlib-nano
    --specs=nosys.specs              # No system calls
)

#
# Post-Build: Generate Binary Files for Flashing
#
find_program(ARM_SIZE arm-none-eabi-size REQUIRED)
find_program(ARM_OBJCOPY arm-none-eabi-objcopy REQUIRED)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    # Generate Intel HEX format
    COMMAND ${ARM_OBJCOPY} -O ihex
        ${PROJECT_NAME}.elf
        ${PROJECT_NAME}.hex
    COMMENT "Generating HEX file..."

    # Generate binary format
    COMMAND ${ARM_OBJCOPY} -O binary
        ${PROJECT_NAME}.elf
        ${PROJECT_NAME}.bin
    COMMENT "Generating BIN file..."

    # Show size information
    COMMAND ${ARM_SIZE} --format=berkeley ${PROJECT_NAME}.elf
    COMMENT "Size analysis:"
)

#
# Flash Target (Optional: Program the MCU)
#
find_program(OPENOCD openocd)
if(OPENOCD)
    add_custom_target(flash
        COMMAND ${OPENOCD}
            -f interface/stlink.cfg
            -f target/stm32f4x.cfg
            -c "program ${PROJECT_NAME}.elf verify reset exit"
        DEPENDS ${PROJECT_NAME}.elf
        COMMENT "Flashing ${PROJECT_NAME}.elf to STM32..."
    )
endif()

#
# Debug Target (Optional: Start GDB server)
#
if(OPENOCD)
    add_custom_target(debug
        COMMAND ${OPENOCD}
            -f interface/stlink.cfg
            -f target/stm32f4x.cfg
        COMMENT "Starting OpenOCD debug server..."
    )
endif()

#
# Build Information
#
message(STATUS "")
message(STATUS "=================================")
message(STATUS "${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "=================================")
message(STATUS "Target MCU:        ${MCU_MODEL}")
message(STATUS "MCU Family:        ${MCU_FAMILY}")
message(STATUS "Toolchain:         ARM GCC")
message(STATUS "Linker Script:     ${MCU_LINKER_SCRIPT}")
message(STATUS "Flash tool:        ${OPENOCD}")
message(STATUS "=================================")
message(STATUS "")
