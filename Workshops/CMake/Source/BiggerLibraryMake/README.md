# BiggerLibraryMake - Step 4

**CMake Workshop - HAN University**
**Makefiles: Better, But Not Perfect**

---

## What This Is

Same project as BiggerLibrary, but now with a **Makefile** for build automation. This is **Step 4** of the CMake workshop, showing how Makefiles help... and where they fall short.

---

## Files

```
BiggerLibraryMake/
├── README.md              # This file
├── Makefile              # Build automation! 🎉
├── include/              # All header files
│   ├── sht45.hpp
│   ├── i2c.hpp
│   ├── logger.hpp
│   ├── config.hpp
│   └── filter.hpp
├── main.cpp
├── sht45.cpp
├── i2c.cpp
├── logger.cpp
├── config.cpp
└── filter.cpp
```

---

## What You'll Learn

- ✅ Makefiles automate build processes
- ✅ Incremental compilation (only rebuild changed files)
- ✅ Dependency tracking
- ❌ But... Makefiles have problems too
- Why we need something better (CMake!)

---

## How to Build

### Build with Make

**Simple commands! No more typing long g++ commands!**

```bash
make              # Build the project
```

**That's it!** The Makefile handles all the complexity.

### Other Useful Commands

```bash
make clean        # Remove all build artifacts
make rebuild      # Clean + build from scratch
make run          # Build and run the application
make help         # Show available commands
```

### Run the Program

```bash
./sensor_app
```

Or:
```bash
make run
```

**Expected Output:**
```
[LOG] Starting sensor application...
Temperature: 20.25
[LOG] Application complete
```

---

## The Good News! ✅

### 1. **No More Long Commands**
Instead of typing:
```bash
g++ main.cpp sht45.cpp i2c.cpp logger.cpp config.cpp filter.cpp -I./include -std=c++17 -Wall -O2 -o sensor_app
```

Just type:
```bash
make
```

**Much better!**

### 2. **Incremental Builds**
Change just `logger.cpp`?

```bash
make
```

**Output:**
```
g++ -std=c++17 -Wall -O2 -I./include -c logger.cpp
g++ main.o sht45.o i2c.o logger.o config.o filter.o -o sensor_app
Build complete: sensor_app
```

**Only recompiles `logger.cpp`** - not all 6 files! Fast!

### 3. **Dependency Tracking**
Change `sht45.hpp`?

Make knows that `main.cpp` and `sht45.cpp` depend on it, and recompiles only those files!

### 4. **Reusable**
Other developers just type `make`. No need to remember complex commands!

### 5. **Clean Builds**
```bash
make clean        # Remove .o files and executable
make rebuild      # Start fresh
```

---

## The Bad News ❌

**But... Makefiles aren't perfect!**

### Problem 1: Repetitive Rules

Look at the Makefile:

```makefile
main.o: main.cpp $(HEADERS)
    $(CXX) $(CXXFLAGS) -c main.cpp

sht45.o: sht45.cpp include/sht45.hpp
    $(CXX) $(CXXFLAGS) -c sht45.cpp

i2c.o: i2c.cpp include/i2c.hpp
    $(CXX) $(CXXFLAGS) -c i2c.cpp

# ... 3 more nearly identical rules!
```

**6 almost identical rules!** Every `.cpp` file needs its own rule. Tedious!

### Problem 2: Manual File Listing

You must manually list:
```makefile
SOURCES = main.cpp sht45.cpp i2c.cpp logger.cpp config.cpp filter.cpp
OBJECTS = main.o sht45.o i2c.o logger.o config.o filter.o
```

Add a new file? **Update 2-3 places in the Makefile!**

### Problem 3: Manual Dependency Tracking

```makefile
main.o: main.cpp $(HEADERS)  # Which headers does main.cpp actually use?
sht45.o: sht45.cpp include/sht45.hpp  # Does it include others?
```

You have to manually specify dependencies. Miss one = stale builds!

### Problem 4: Platform-Specific

**This Makefile only works on Linux/Mac!**

Windows users? They need:
- Different Makefile syntax
- Different compiler (cl.exe instead of g++)
- Different flags
- Different tools (nmake instead of make)

Want cross-platform? **Write multiple Makefiles!**

### Problem 5: IDE Integration

**Modern IDEs** (VS Code, CLion, Visual Studio) can't easily understand Makefiles:
- No automatic project detection
- Limited code completion
- No integrated debugging setup

### Problem 6: Hard-Coded Compiler

```makefile
CXX = g++
```

What if someone wants to use:
- `clang++` instead of `g++`?
- MSVC on Windows?
- Cross-compiler for embedded systems?

**Have to edit the Makefile manually!**

---

## Inside the Makefile

Let's understand what's happening:

### Variables
```makefile
CXX = g++                           # Compiler
CXXFLAGS = -std=c++17 -Wall -O2 -I./include  # Flags
TARGET = sensor_app                 # Output executable
```

### Compilation Rules
```makefile
# Pattern: target: dependencies
#     command

main.o: main.cpp $(HEADERS)
    $(CXX) $(CXXFLAGS) -c main.cpp
```

Means:
- To create `main.o`, we need `main.cpp` and headers
- If any dependency changed, run the command
- `-c` means "compile only, don't link"

### Linking Rule
```makefile
$(TARGET): $(OBJECTS)
    $(CXX) $(OBJECTS) -o $(TARGET)
```

Means:
- To create `sensor_app`, we need all `.o` files
- Link them together into the executable

---

## Makefile Anatomy

**62 lines of Makefile code!** Let's break it down:

```makefile
# Lines 1-10: Comments and setup
CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2 -I./include

# Lines 11-15: Variables (manual listing)
SOURCES = main.cpp sht45.cpp i2c.cpp logger.cpp config.cpp filter.cpp
OBJECTS = main.o sht45.o i2c.o logger.o config.o filter.o
HEADERS = include/sht45.hpp include/i2c.hpp ...

# Lines 16-20: Linking rule
$(TARGET): $(OBJECTS)
    $(CXX) $(OBJECTS) -o $(TARGET)

# Lines 21-50: Compilation rules (repetitive!)
main.o: main.cpp $(HEADERS)
    $(CXX) $(CXXFLAGS) -c main.cpp
sht45.o: sht45.cpp include/sht45.hpp
    $(CXX) $(CXXFLAGS) -c sht45.cpp
# ... 4 more similar rules ...

# Lines 51-62: Utility targets
clean:
    rm -f $(OBJECTS) $(TARGET)
```

**Notice the repetition?** This is what CMake will solve!

---

## The Question

**"Makefiles are better than manual compilation, but... there must be an even better way!"** 🤔

**Yes! CMake!**

CMake generates Makefiles for you, but also:
- Generates Visual Studio projects on Windows
- Generates Xcode projects on macOS
- Automatically detects dependencies
- No repetitive rules
- Cross-platform by design

---

## Exercise

**Try modifying the Makefile to understand it:**

### Exercise 1: Add a New Source File

1. Create `humidity.cpp` and `humidity.hpp`
2. Update the Makefile:
   - Add to `SOURCES` variable
   - Add to `OBJECTS` variable
   - Add to `HEADERS` variable
   - Add new compilation rule for `humidity.o`

**How many places did you have to edit?** (Hint: At least 4!)

### Exercise 2: Change Compiler

1. Edit Makefile: `CXX = clang++`
2. Run `make clean && make`
3. See it work with clang instead of g++

**But what if you want to switch back?** Edit again!

### Exercise 3: Understand Dependencies

1. Run `make clean && make` - builds everything
2. Touch a header: `touch include/sht45.hpp`
3. Run `make` again
4. **Which files recompiled?** Only those depending on `sht45.hpp`!

---

## Comparison

| Feature | Manual (Step 3) | Makefile (Step 4) |
|---------|----------------|-------------------|
| Command length | 😫 Very long | ✅ Just `make` |
| Incremental builds | ❌ No | ✅ Yes |
| Dependency tracking | ❌ Manual | ✅ Yes |
| Cross-platform | ❌ No | ❌ No |
| Repetitive | ❌ Very | ⚠️ Somewhat |
| Maintenance | 😫 Terrible | ⚠️ Tedious |
| IDE integration | ❌ No | ⚠️ Limited |

**Better, but not perfect!**

---

## Next Step

Ready to see the ultimate solution?

Continue to: `../BiggerLibraryCMake/` (Step 5 - CMake solves everything!)

Or return to: `../../README.md` (Workshop overview)

---

**Makefiles are better... but CMake is best!** 🎯
