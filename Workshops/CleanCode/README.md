# Clean Code & MISRA C++ Workshop

**HAN University of Applied Sciences**
**Software Engineering - Programming 6**
**Johan Korten - v1.0 2025**

---

## Workshop Overview

This workshop teaches you how to write clean, maintainable, and safe C++ code following industry-standard guidelines. You'll learn to identify code smells, apply MISRA C++ rules, and use automated tools to improve code quality.

### Learning Objectives

By the end of this workshop, you will be able to:
- ✅ Identify common code smells in C++ code
- ✅ Apply MISRA C++ 2023 guidelines
- ✅ Write clean, maintainable code following best practices
- ✅ Use automated code smell detection tools
- ✅ Refactor problematic code to meet quality standards
- ✅ Understand the importance of code quality in professional development

### Prerequisites

- Basic C++ programming knowledge
- Understanding of object-oriented programming
- Familiarity with compilation and build processes
- Text editor or IDE

---

## Workshop Structure

```
CleanCode/
├── README.md                    # This file - Workshop overview
├── Keynotes/                    # Presentation materials
│   ├── Workshop Clean Code 2025.key   # Keynote (macOS)
│   ├── Workshop Clean Code 2025.pdf   # PDF (universal)
│   └── Workshop Clean Code 2025.pptx  # PowerPoint (Windows)
└── MISRA_CPP_23/               # Workshop materials
    ├── MISRA Rules/            # MISRA C++ 2023 reference
    │   └── MISRA_CPP_2023.md
    ├── Example/                # Example code with violations
    │   └── our_wonderful_code.cpp
    ├── MoreCode/               # Additional practice examples
    │   ├── rgtData/
    │   ├── algemeen/
    │   └── RGT.cmake
    ├── SmellScanner/           # Code smell detection tool
    │   ├── code_smell_scanner.py
    │   ├── CODE_SMELL_SCANNER_README.md
    │   ├── CODE_SMELL_SCANNER_GUIDE.md
    │   └── CODE_SMELL_QUICK_START.md
    ├── scan_all_and_summarize.py
    └── scan_configurable.py
```

---

## Part 1: Code Smells

### What are Code Smells?

Code smells are indicators of poor code quality that suggest deeper problems. They don't necessarily mean the code doesn't work, but they make it:
- Harder to understand
- Harder to maintain
- More prone to bugs
- Difficult to extend

### Common Code Smells Covered

#### 🔴 Critical Severity
- **Missing destructors** - Memory leaks waiting to happen
- **Goto statements** - Unstructured control flow
- **Uninitialized variables** - Undefined behavior

#### 🟡 Major Severity
- **Long methods** - Methods doing too much
- **God objects** - Classes with too many responsibilities
- **Deep nesting** - Complex, hard-to-follow logic
- **Missing const** - Lack of immutability guarantees

#### 🔵 Minor Severity
- **Magic numbers** - Unexplained literal values
- **Long parameter lists** - Too many function parameters
- **Inconsistent naming** - Poor code readability

### Example: Spotting Code Smells

Look at `Example/our_wonderful_code.cpp`:

```cpp
class SensorArray {
    int* data;
    int size;
public:
    SensorArray(int s) {
        size = s;
        data = new int[size];  // ❌ Missing destructor - memory leak!
    }

    int getValue(int index) {  // ❌ Missing const
        return data[index];     // ❌ No bounds checking
    }

    void process() {
        int result;             // ❌ Uninitialized variable
        for (int i = 0; i <= size; i++) {  // ❌ Off-by-one error
            if (data[i] < 0)
                goto error;     // ❌ Goto statement!
            result = data[i] * 2;  // ❌ Magic number
        }
        return;
    error:                      // ❌ Error handling with goto
        result = -1;            // ❌ Magic number
    }
};

void main() {  // ❌ Should be 'int main()'
    SensorArray sensors(MAX_SENSORS);
    sensors.process();
}  // ❌ Memory leak - destructor never called
```

**How many code smells can you find?**

---

## Part 2: MISRA C++ Guidelines

### What is MISRA C++?

MISRA (Motor Industry Software Reliability Association) C++ is a set of coding guidelines designed to ensure:
- **Safety** - Avoid undefined behavior
- **Security** - Prevent common vulnerabilities
- **Portability** - Code works across platforms
- **Maintainability** - Code is easy to understand and modify

### MISRA C++ 2023

This workshop uses the latest MISRA C++ 2023 standard. Key improvements:
- Updated for modern C++ (C++17/20)
- Better alignment with C++ Core Guidelines
- Focus on zero-overhead abstractions
- Emphasis on RAII and modern idioms

### Key MISRA Rules Covered

See `MISRA_CPP_23/MISRA Rules/MISRA_CPP_2023.md` for complete reference.

**Essential rules include:**

- **Rule 3-9-1**: Explicit constructors and destructors
- **Rule 5-0-4**: Use fixed-width integer types (`uint8_t`, `int32_t`)
- **Rule 7-1-1**: Declare methods `const` where appropriate
- **Rule 7-3-1**: No uninitialized variables
- **Rule 16-2-2**: No magic numbers
- **Rule 18-0-3**: Avoid C-style casts

---

## Part 3: Code Smell Scanner Tool

### Overview

The Code Smell Scanner is a Python-based tool that automatically detects code quality issues in C++ files.

### Quick Start

```bash
# Navigate to the scanner directory
cd MISRA_CPP_23/SmellScanner

# Scan a single file
python3 code_smell_scanner.py ../Example/our_wonderful_code.cpp

# Scan entire directory
python3 code_smell_scanner.py ../MoreCode/

# Detailed report
python3 code_smell_scanner.py ../MoreCode/ -v

# Extra details with code snippets
python3 code_smell_scanner.py ../MoreCode/ -vv
```

### Scanner Features

✅ **Detects:**
- Magic numbers
- Long methods (>50 lines)
- Deep nesting (>4 levels)
- Long parameter lists (>5 parameters)
- Missing destructors
- Goto statements
- Uninitialized variables
- Missing const qualifiers
- C-style casts

✅ **Provides:**
- Severity ratings (Critical/Major/Minor)
- File and line number references
- Code snippets showing violations
- Summary statistics
- Actionable recommendations

### Verbosity Levels

**Level 0: Summary (default)**
```bash
python3 code_smell_scanner.py ../MoreCode/
```
Shows total counts and breakdown by severity/type

**Level 1: File Details (-v)**
```bash
python3 code_smell_scanner.py ../MoreCode/ -v
```
Lists all issues with file names and line numbers

**Level 2: Code Snippets (-vv)**
```bash
python3 code_smell_scanner.py ../MoreCode/ -vv
```
Shows actual code causing each violation

### Documentation

Detailed guides available in `MISRA_CPP_23/SmellScanner/`:
- **CODE_SMELL_QUICK_START.md** - Get started in 30 seconds
- **CODE_SMELL_SCANNER_GUIDE.md** - Complete usage guide
- **CODE_SMELL_SCANNER_README.md** - Technical documentation

---

## Part 4: Hands-On Exercises

### Exercise 1: Identify Code Smells

**Task:** Analyze `Example/our_wonderful_code.cpp`

1. Read the code manually
2. List all code smells you can find
3. Run the scanner to verify:
   ```bash
   cd MISRA_CPP_23/SmellScanner
   python3 code_smell_scanner.py ../Example/our_wonderful_code.cpp -vv
   ```
4. Compare your findings with the scanner results

**Questions:**
- Which smells are most critical?
- What could go wrong at runtime?
- How would you fix each issue?

### Exercise 2: Refactor Bad Code

**Task:** Fix all violations in `our_wonderful_code.cpp`

Create a new file `our_wonderful_code_fixed.cpp` that:
- ✅ Has proper RAII (destructor, or use `std::vector`)
- ✅ Uses const where appropriate
- ✅ Eliminates goto statements
- ✅ Initializes all variables
- ✅ Replaces magic numbers with named constants
- ✅ Has proper error handling
- ✅ Uses `int main()` not `void main()`

**Verify your solution:**
```bash
python3 code_smell_scanner.py our_wonderful_code_fixed.cpp -vv
```
Goal: Zero code smells!

### Exercise 3: Scan Real Code

**Task:** Analyze code in `MoreCode/` directory

1. Run summary scan:
   ```bash
   python3 code_smell_scanner.py ../MoreCode/
   ```

2. Analyze the results:
   - How many total code smells?
   - Which severity is most common?
   - Which files have the most issues?

3. Pick one file and refactor it to reduce smells by 50%

4. Verify improvement:
   ```bash
   python3 code_smell_scanner.py ../MoreCode/[your_file] -vv
   ```

### Exercise 4: Write Clean Code From Scratch

**Task:** Implement a new class following all guidelines

Create a `TemperatureSensor` class that:
- Reads temperature values
- Validates input ranges
- Filters outliers
- Follows MISRA C++ rules
- Has zero code smells

**Requirements:**
- Use RAII
- Use const correctness
- Use fixed-width types
- No magic numbers
- Proper error handling
- Well-documented

**Template:**
```cpp
#include <cstdint>
#include <vector>

/// @brief Temperature sensor with data validation
class TemperatureSensor {
public:
    TemperatureSensor();
    ~TemperatureSensor() = default;

    // Add your methods here

private:
    static constexpr float MIN_TEMP = -40.0F;
    static constexpr float MAX_TEMP = 85.0F;

    // Add your members here
};
```

---

## Part 5: Best Practices

### The Clean Code Principles

#### 1. **Meaningful Names**
```cpp
// ❌ Bad
int d;  // elapsed time in days

// ✅ Good
int elapsedTimeInDays;
```

#### 2. **Functions Should Do One Thing**
```cpp
// ❌ Bad - does too much
void processAndSaveData() {
    readSensor();
    validateData();
    filterData();
    saveToFile();
    sendToServer();
}

// ✅ Good - single responsibility
void processData() {
    SensorData data = readSensor();
    validateData(data);
    return filterData(data);
}
```

#### 3. **Don't Repeat Yourself (DRY)**
```cpp
// ❌ Bad - repetition
float tempC1 = (tempF1 - 32) * 5 / 9;
float tempC2 = (tempF2 - 32) * 5 / 9;

// ✅ Good - reusable function
float fahrenheitToCelsius(float tempF) {
    static constexpr float FAHRENHEIT_OFFSET = 32.0F;
    static constexpr float CONVERSION_RATIO = 5.0F / 9.0F;
    return (tempF - FAHRENHEIT_OFFSET) * CONVERSION_RATIO;
}
```

#### 4. **RAII - Resource Acquisition Is Initialization**
```cpp
// ❌ Bad - manual memory management
class Buffer {
    char* data;
public:
    Buffer(int size) { data = new char[size]; }
    void cleanup() { delete[] data; }  // Easy to forget!
};

// ✅ Good - automatic cleanup
class Buffer {
    std::vector<char> data;
public:
    Buffer(int size) : data(size) {}
    // Destructor automatic, no memory leaks!
};
```

#### 5. **Const Correctness**
```cpp
// ❌ Bad - missing const
class Sensor {
    float value;
public:
    float getValue() { return value; }  // Can modify object!
};

// ✅ Good - explicit const
class Sensor {
    float value;
public:
    float getValue() const { return value; }  // Read-only
};
```

---

## Part 6: MISRA Compliance Checklist

Use this checklist when writing C++ code:

### Memory Management
- [ ] All classes with `new` have destructors (or use RAII)
- [ ] No manual `new`/`delete` (use smart pointers or containers)
- [ ] No memory leaks (verified with tools)

### Type Safety
- [ ] Use fixed-width types (`uint8_t`, `int32_t`)
- [ ] No C-style casts (use `static_cast`, etc.)
- [ ] No implicit conversions

### Initialization
- [ ] All variables initialized before use
- [ ] Member variables initialized in constructor
- [ ] Use initialization lists where appropriate

### Functions
- [ ] Methods marked `const` where appropriate
- [ ] No more than 5 parameters
- [ ] No more than 50 lines per function

### Control Flow
- [ ] No `goto` statements
- [ ] Maximum nesting depth of 4
- [ ] No fall-through in switch statements (use `[[fallthrough]]` if intentional)

### Constants
- [ ] No magic numbers (use named constants)
- [ ] Use `constexpr` for compile-time constants
- [ ] Constants use UPPERCASE or clear names

### Error Handling
- [ ] No error codes (use exceptions or error objects)
- [ ] Validate all inputs
- [ ] Handle all error conditions

---

## Part 7: Common Violations and Fixes

### Violation 1: Missing Destructor
```cpp
// ❌ Violation
class Buffer {
    char* data;
public:
    Buffer(int size) { data = new char[size]; }
    // Missing destructor - memory leak!
};

// ✅ Fix 1: Add destructor (Rule of Five)
class Buffer {
    char* data;
public:
    Buffer(int size) : data(new char[size]) {}
    ~Buffer() { delete[] data; }
    Buffer(const Buffer&) = delete;  // Or implement properly
    Buffer& operator=(const Buffer&) = delete;
};

// ✅ Fix 2: Use RAII (Better!)
class Buffer {
    std::vector<char> data;
public:
    Buffer(int size) : data(size) {}
    // Automatic cleanup, no destructor needed
};
```

### Violation 2: Magic Numbers
```cpp
// ❌ Violation
float convertTemp(float f) {
    return (f - 32) * 5 / 9;  // What are 32, 5, 9?
}

// ✅ Fix
float convertTemp(float fahrenheit) {
    static constexpr float FAHRENHEIT_OFFSET = 32.0F;
    static constexpr float CELSIUS_TO_FAHRENHEIT_RATIO = 5.0F / 9.0F;
    return (fahrenheit - FAHRENHEIT_OFFSET) * CELSIUS_TO_FAHRENHEIT_RATIO;
}
```

### Violation 3: Missing Const
```cpp
// ❌ Violation
class Sensor {
    float temperature;
public:
    float getTemperature() {  // Should be const!
        return temperature;
    }
};

// ✅ Fix
class Sensor {
    float temperature;
public:
    float getTemperature() const {  // Read-only method
        return temperature;
    }
};
```

### Violation 4: Goto Statement
```cpp
// ❌ Violation
void process() {
    for (int i = 0; i < 10; i++) {
        if (error)
            goto cleanup;
    }
    return;
cleanup:
    handleError();
}

// ✅ Fix
void process() {
    for (int i = 0; i < 10; i++) {
        if (error) {
            handleError();
            return;
        }
    }
}
```

---

## Workshop Timeline

### Total Duration: 120 minutes (2 hours)

#### Part 1: Introduction (15 min)
- Code quality importance
- Code smells overview
- MISRA C++ introduction

#### Part 2: Theory (20 min)
- Common code smells with examples
- MISRA rules explained
- Clean code principles

#### Part 3: Tool Introduction (15 min)
- Code smell scanner demo
- How to use the tool
- Interpreting results

#### Part 4: Hands-On Practice (40 min)
- Exercise 1: Identify smells (10 min)
- Exercise 2: Refactor code (20 min)
- Exercise 3: Scan real code (10 min)

#### Part 5: Advanced Topics (20 min)
- Best practices discussion
- Real-world examples
- Q&A

#### Part 6: Wrap-up (10 min)
- Key takeaways
- Resources for further learning
- Next steps (CMake workshop)

---

## Additional Resources

### Documentation in This Workshop
- `MISRA_CPP_23/MISRA Rules/MISRA_CPP_2023.md` - Complete MISRA reference
- `MISRA_CPP_23/SmellScanner/CODE_SMELL_SCANNER_GUIDE.md` - Tool guide
- `Keynotes/` - Workshop presentation materials

### External Resources

#### MISRA C++
- [MISRA C++ Official](https://misra.org.uk/)
- [MISRA C++ 2023 Guidelines](https://misra.org.uk/product/misra-cpp2023/)

#### Clean Code
- "Clean Code" by Robert C. Martin
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/)
- [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)

#### Tools
- Static analyzers: Clang-Tidy, Cppcheck
- Dynamic analyzers: Valgrind, AddressSanitizer
- Code formatters: clang-format

---

## Tools Setup

### Code Smell Scanner Requirements

**Python 3.6+** required:
```bash
# Check Python version
python3 --version

# macOS (Homebrew)
brew install python3

# Ubuntu/Debian
sudo apt-get install python3

# Windows
# Download from python.org
```

**No external dependencies required!** The scanner uses only Python standard library.

### Running the Scanner

```bash
# From SmellScanner directory
cd MISRA_CPP_23/SmellScanner

# Scan single file
python3 code_smell_scanner.py path/to/file.cpp

# Scan directory recursively
python3 code_smell_scanner.py path/to/directory/

# With verbosity
python3 code_smell_scanner.py path/to/directory/ -v   # Detailed
python3 code_smell_scanner.py path/to/directory/ -vv  # Extra detailed
```

---

## Assessment Criteria

Students will be evaluated on:

### Knowledge (40%)
- [ ] Can identify common code smells
- [ ] Understands MISRA C++ principles
- [ ] Knows when to apply specific rules

### Skills (40%)
- [ ] Can use the code smell scanner effectively
- [ ] Can refactor code to eliminate violations
- [ ] Writes clean code from scratch

### Application (20%)
- [ ] Applies principles to real code
- [ ] Makes informed decisions about code quality
- [ ] Can justify refactoring choices

---

## Next Steps

After completing this workshop:

1. **Practice** - Scan and refactor your own code
2. **CMake Workshop** - Learn modern C++ build systems
3. **Advanced Topics** - Static analysis, design patterns
4. **Real Projects** - Apply principles to your projects

---

## Troubleshooting

### Scanner Issues

**Problem:** Scanner not finding files
```bash
# Make sure you're in the right directory
cd MISRA_CPP_23/SmellScanner

# Use absolute paths if needed
python3 code_smell_scanner.py /full/path/to/file.cpp
```

**Problem:** Python not found
```bash
# Try python instead of python3
python code_smell_scanner.py file.cpp

# Or specify full path
/usr/bin/python3 code_smell_scanner.py file.cpp
```

### Common Questions

**Q: Do I need to fix ALL code smells?**
A: Prioritize by severity. Fix Critical first, then Major, then Minor as time permits.

**Q: Are there exceptions to MISRA rules?**
A: Yes, but they must be justified and documented. Use deviations sparingly.

**Q: Can I use modern C++ features?**
A: Yes! MISRA C++ 2023 embraces modern C++ (C++17/20) when used safely.

---

## Summary

### Key Takeaways

✅ **Code Quality Matters**
- Prevents bugs before they happen
- Makes code maintainable
- Reduces technical debt

✅ **MISRA C++ Provides Guidelines**
- Safety-critical standards
- Industry best practices
- Proven in embedded systems

✅ **Tools Help Enforce Quality**
- Automated detection
- Consistent standards
- Immediate feedback

✅ **Clean Code is Professional Code**
- Required in industry
- Expected by colleagues
- Valued by employers

---

## Questions?

If you have questions during the workshop:
1. Check the documentation in `MISRA_CPP_23/`
2. Ask your instructor
3. Consult the presentation in `Keynotes/`

---

**Write Clean Code!** ✨

---

*This workshop is part of the Software Engineering curriculum at HAN University of Applied Sciences. It prepares students for the CMake workshop and professional C++ development.*
