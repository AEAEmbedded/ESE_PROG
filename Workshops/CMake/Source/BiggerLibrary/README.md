# BiggerLibrary - Step 3

**CMake Workshop - HAN University**
**The Pain Point: Manual Compilation at Scale**

---

## What This Is

This is a **realistic embedded project** with 6 source files and organized headers. This is **Step 3** of the CMake workshop, designed to make you feel the pain of manual compilation before showing you the solution!

---

## Files

```
BiggerLibrary/
├── README.md              # This file
├── include/              # All header files
│   ├── sht45.hpp         # Temperature sensor
│   ├── i2c.hpp           # I2C communication
│   ├── logger.hpp        # Logging utility
│   ├── config.hpp        # Configuration
│   └── filter.hpp        # Signal filtering
├── main.cpp              # Main application
├── sht45.cpp             # Sensor implementation
├── i2c.cpp               # I2C implementation
├── logger.cpp            # Logger implementation
├── config.cpp            # Config implementation
└── filter.cpp            # Filter implementation
```

**6 source files + 5 headers = Build system needed!**

---

## What You'll Learn

- 😫 **Manual compilation is painful at scale**
- Why build systems exist
- The problems we're trying to solve
- Appreciation for automation!

---

## How to Build

### The Long, Painful Command

Take a deep breath and type this:

```bash
g++ main.cpp sht45.cpp i2c.cpp logger.cpp config.cpp filter.cpp \
    -I./include -std=c++17 -Wall -O2 -o sensor_app
```

**Breakdown:**
- `main.cpp sht45.cpp i2c.cpp logger.cpp config.cpp filter.cpp` - **All 6 source files** (miss one = linker error!)
- `-I./include` - Tell compiler where to find headers
- `-std=c++17` - Use C++17 standard
- `-Wall` - Enable all warnings
- `-O2` - Optimization level 2
- `-o sensor_app` - Output executable

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

## The Problems (This is the point!)

### 😫 Problem 1: Long Command
Every time you want to rebuild, you type this monster command. Copy-paste from notes? Not professional!

### 😫 Problem 2: Easy to Forget Files
Add a new `.cpp` file? Better remember to add it to the command! Forget = mysterious linker errors.

### 😫 Problem 3: No Incremental Builds
Changed one line in `logger.cpp`? Recompile **all 6 files**. Wasteful!

### 😫 Problem 4: No Dependency Tracking
Changed `sht45.hpp`? You need to remember which `.cpp` files include it and recompile those. Miss one = subtle bugs!

### 😫 Problem 5: Error-Prone
Typo in a filename? Wrong flag? Start over!

### 😫 Problem 6: Not Portable
This works on Linux/Mac. Windows with MSVC? Completely different command!

### 😫 Problem 7: Hard to Share
New team member: "How do I build this?" You: "Well, you type this 200-character command..."

---

## The Question

**"There must be a better way!"** 🤔

Yes! That's why build systems exist!

---

## Project Structure

This is a realistic embedded sensor application:

### Core Modules

**SHT45 Sensor** (`sht45.cpp/hpp`)
- Reads temperature from sensor
- Simulated for workshop purposes

**I2C Communication** (`i2c.cpp/hpp`)
- Low-level I2C bus interface
- Initialized via RAII in constructor

**Logger** (`logger.cpp/hpp`)
- Logging utility for debugging
- Outputs to stdout/stderr

**Config** (`config.cpp/hpp`)
- Configuration management
- Loads settings in constructor (RAII)

**Filter** (`filter.cpp/hpp`)
- Signal filtering for sensor data
- Simple coefficient-based filter

### Main Application

Puts it all together:
1. Initialize all components (RAII - no explicit init calls!)
2. Read temperature from sensor
3. Apply filtering
4. Display result
5. Log completion

---

## Code Quality Features

All code follows professional standards:

### ✅ MISRA C++ Compliant
- No magic numbers
- Fixed-width types (`uint8_t`, `int32_t`)
- No goto statements
- Proper const correctness

### ✅ Clean Code Principles
- Meaningful names
- Single responsibility
- Self-documenting
- Well-commented

### ✅ Modern C++ (Bjarne Stroustrup)
- **RAII** - Resources managed in constructors/destructors
- **Rule of Five** - Explicit copy/move semantics
- **Const correctness** - Methods properly marked `const`
- **No two-phase initialization** - Objects ready after construction

### Example: RAII in Action

```cpp
int main() {
    // RAII: All objects fully initialized and ready to use
    Logger logger;
    Config config;      // Config loaded in constructor
    I2C i2c;           // I2C bus initialized in constructor
    Filter filter;
    SHT45 sensor;

    // No init() calls needed - everything is ready!
    logger.log("Starting sensor application...");
    // ...
}  // Automatic cleanup - destructors handle everything
```

---

## Common Build Errors

### Forgot a source file
```bash
g++ main.cpp sht45.cpp i2c.cpp logger.cpp config.cpp -I./include -o sensor_app
```
**Error:** Undefined reference to `Filter::apply(float)`
**Fix:** Add `filter.cpp` to the command

### Forgot include directory
```bash
g++ main.cpp sht45.cpp i2c.cpp logger.cpp config.cpp filter.cpp -o sensor_app
```
**Error:** fatal error: 'sht45.hpp' file not found
**Fix:** Add `-I./include`

### Wrong C++ standard
```bash
g++ main.cpp sht45.cpp i2c.cpp logger.cpp config.cpp filter.cpp -I./include -o sensor_app
```
**Error:** warning: constexpr if is a C++17 extension
**Fix:** Add `-std=c++17`

---

## Exercise

**Try these to feel the pain:**

1. **Modify `logger.cpp`** - Add a timestamp to log messages
   - Rebuild with the full command
   - Notice it recompiles **all** files, even ones you didn't touch

2. **Add a new file** - Create `humidity.cpp/hpp`
   - Update main.cpp to use it
   - Remember to add to compile command!
   - Forget? Linker error!

3. **Change a header** - Modify `config.hpp`
   - Which `.cpp` files need recompiling?
   - You have to remember manually!

**Frustrating, right? That's the point!**

---

## Next Step

Ready for some relief?

Continue to: `../BiggerLibraryMake/` (Step 4 - Makefiles help... sort of)

Or return to: `../../README.md` (Workshop overview)

---

**Now you understand why build systems exist!** 💡
