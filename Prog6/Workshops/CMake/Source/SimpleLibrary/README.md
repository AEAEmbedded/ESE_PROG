# SimpleLibrary - Step 2

**CMake Workshop - HAN University**
**Multiple Files: The Complexity Begins**

---

## What This Is

This project introduces **multiple files** - a header, an implementation, and a main file. This is **Step 2** of the CMake workshop, showing how compilation gets more complex.

---

## Files

```
SimpleLibrary/
├── README.md           # This file
├── main.cpp           # Main application
├── sht45.hpp          # Header file (class declaration)
└── sht45.cpp          # Implementation file (class definition)
```

---

## What You'll Learn

- Separating interface (`.hpp`) from implementation (`.cpp`)
- Why you need to compile multiple files
- That listing files manually gets tedious quickly

---

## How to Build

### Compile with g++

Now you need to list **all** `.cpp` files:

```bash
g++ main.cpp sht45.cpp -o sensor_app
```

**Breakdown:**
- `g++` - The C++ compiler
- `main.cpp sht45.cpp` - **All** source files (you must list every one!)
- `-o sensor_app` - Output executable

**Forget to list a file?** Linker errors!

### Run the Program

```bash
./sensor_app
```

**Expected Output:**
```
Temperature: 22.5
```

---

## The Lesson

**Getting more annoying!**

With just 2 `.cpp` files, it's already tedious:
- Have to remember which files to compile
- Easy to forget one
- What if you add a new file?

Imagine if you had 10 files... 50 files... 100 files!

**There must be a better way!**

Move on to **Step 3: BiggerLibrary** to feel the real pain!

---

## Code Structure

### Header File (`sht45.hpp`)
- Class declaration
- Public interface
- Method signatures

### Implementation File (`sht45.cpp`)
- Class definition
- Method implementations
- Actual code logic

### Main File (`main.cpp`)
- Uses the SHT45 class
- `#include "sht45.hpp"` to access the interface

---

## Code Quality

This example demonstrates:
- ✅ **RAII** - Objects fully initialized in constructor
- ✅ **Const correctness** - `read_temperature()` is `const`
- ✅ **No magic numbers** - `SIMULATED_TEMPERATURE` constant
- ✅ **Rule of Five** - Explicit default operations
- ✅ **Fixed-width types** - Using standard types
- ✅ **Documentation** - Clear comments

---

## Common Mistakes

### Forgot to include implementation file
```bash
g++ main.cpp -o sensor_app  # ❌ Missing sht45.cpp
```
**Error:** Undefined reference to `SHT45::read_temperature()`

### Wrong order (doesn't matter for compilation, but good practice)
```bash
g++ sht45.cpp main.cpp -o sensor_app  # ✅ Works, but unconventional
g++ main.cpp sht45.cpp -o sensor_app  # ✅ Better (main first)
```

---

## Next Step

Continue to: `../BiggerLibrary/` (Step 3 - Feel the pain!)

Or return to: `../../README.md` (Workshop overview)

---

**The complexity is building...** 📈
