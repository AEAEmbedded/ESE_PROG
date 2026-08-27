# Code Smell Scanner - User Guide

**Version:** 1.0.0
**Date:** November 18, 2025
**Purpose:** Detect code quality issues and design problems in C++ code

---

## 🎯 What is a Code Smell?

A **code smell** is a surface indication that usually corresponds to a deeper problem in the system. Code smells are not bugs—they are not technically incorrect and do not prevent the program from functioning. Instead, they indicate weaknesses in design that may slow down development or increase the risk of bugs or failures.

---

## 📊 Code Smells Detected

### 1. **Long Method** 🔴 MAJOR
Methods that are too long (>50 lines)

**Problem:** Hard to understand, test, and maintain
**Solution:** Break down using Extract Method refactoring

**Example:**
```cpp
// BAD - 80 line method
void processData() {
    // 80 lines of code...
}

// GOOD - Extracted methods
void processData() {
    validateInput();
    transformData();
    writeOutput();
}
```

---

### 2. **Long Parameter List** 🟡 MAJOR
Functions with too many parameters (>5)

**Problem:** Hard to remember, prone to errors
**Solution:** Use parameter object or builder pattern

**Example:**
```cpp
// BAD
void createUser(string name, string email, int age,
                string address, string phone, string city) { }

// GOOD - Parameter object
struct UserData {
    string name, email, address, phone, city;
    int age;
};
void createUser(const UserData& data) { }
```

---

### 3. **Deep Nesting** 🟡 MAJOR
Code nested too deeply (>4 levels)

**Problem:** Hard to follow logic, error-prone
**Solution:** Extract nested logic or use early returns

**Example:**
```cpp
// BAD - Deep nesting
if (condition1) {
    if (condition2) {
        if (condition3) {
            if (condition4) {
                if (condition5) {
                    // Too deep!
                }
            }
        }
    }
}

// GOOD - Early returns
if (!condition1) return;
if (!condition2) return;
if (!condition3) return;
if (!condition4) return;
if (!condition5) return;
// Logic here
```

---

### 4. **Magic Number** 🔵 MINOR
Unexplained numeric literals in code

**Problem:** Unclear meaning, hard to change
**Solution:** Replace with named constants

**Example:**
```cpp
// BAD
if (speed > 299792458) { }

// GOOD
constexpr int SPEED_OF_LIGHT = 299792458;
if (speed > SPEED_OF_LIGHT) { }
```

---

### 5. **Large Class** 🔴 CRITICAL
Classes that are too large (>500 lines)

**Problem:** Violates Single Responsibility Principle
**Solution:** Split into multiple focused classes

**Example:**
```cpp
// BAD - 800 line class doing everything
class SystemManager {
    // Database methods
    // UI methods
    // Network methods
    // Logging methods
    // Configuration methods
    // ... 800 lines
};

// GOOD - Split responsibilities
class DatabaseManager { };
class UIManager { };
class NetworkManager { };
class Logger { };
class ConfigManager { };
```

---

### 6. **God Class** 🔴 CRITICAL
Classes with too many responsibilities (>20 methods, >15 fields)

**Problem:** Knows/does too much, hard to maintain
**Solution:** Apply Single Responsibility Principle

**Example:**
```cpp
// BAD - God class
class Application {
    // 30 methods
    // 25 member variables
    // Handles: DB, UI, network, files, config, logging...
};

// GOOD - Separate concerns
class Application {
    DatabaseManager db;
    UIController ui;
    NetworkClient network;
    // Each handles one concern
};
```

---

### 7. **Dead Code** 🟡 MAJOR
Unreachable or commented out code

**Problem:** Clutters codebase, confusing
**Solution:** Remove (use version control)

**Example:**
```cpp
// BAD - Unreachable
void foo() {
    return;
    doSomething();  // Never executed!
}

// BAD - Commented out
// void oldImplementation() {
//     // 50 lines of old code
// }

// GOOD - Clean
void foo() {
    return;
}
// Old code in git history, not in source
```

---

### 8. **Complex Conditional** 🟡 MAJOR
Conditionals with too many logical operators (>3)

**Problem:** Hard to understand and test
**Solution:** Extract into boolean variables/methods

**Example:**
```cpp
// BAD
if (user != nullptr && user->isActive() &&
    (user->getRole() == ADMIN || user->getRole() == MODERATOR) &&
    user->hasPermission("write") && !user->isBanned()) {
    // Complex!
}

// GOOD
bool isValidActiveUser = user != nullptr && user->isActive();
bool hasAdminRights = user->getRole() == ADMIN || user->getRole() == MODERATOR;
bool canWrite = user->hasPermission("write") && !user->isBanned();

if (isValidActiveUser && hasAdminRights && canWrite) {
    // Clear!
}
```

---

### 9. **Duplicate Code** 🟡 MAJOR
Same/similar code appearing multiple times

**Problem:** Maintenance burden, inconsistency
**Solution:** Extract into shared method

**Example:**
```cpp
// BAD - Duplicate code
void processUserData() {
    validate();
    transform();
    save();
}

void processProductData() {
    validate();  // Duplicate!
    transform(); // Duplicate!
    save();      // Duplicate!
}

// GOOD - Extracted
void processData(Data& data) {
    validate();
    transform();
    save();
}
```

---

### 10. **Data Clump** 🔵 MINOR
Same group of parameters appearing together

**Problem:** Indicates missing abstraction
**Solution:** Create parameter object

**Example:**
```cpp
// BAD - Same parameters everywhere
void drawRect(int x, int y, int width, int height);
void fillRect(int x, int y, int width, int height);
void checkRect(int x, int y, int width, int height);

// GOOD - Parameter object
struct Rectangle {
    int x, y, width, height;
};

void drawRect(const Rectangle& rect);
void fillRect(const Rectangle& rect);
void checkRect(const Rectangle& rect);
```

---

## 🚀 Usage

### Scan Single File
```bash
python3 code_smell_scanner.py file.cpp
```

### Scan Directory
```bash
python3 code_smell_scanner.py MoreCode_Anonymized/
```

### Verbose Mode (Show All Details)
```bash
python3 code_smell_scanner.py file.cpp --verbose
```

### Examples

**Quick scan:**
```bash
python3 code_smell_scanner.py src/
```

**Detailed analysis:**
```bash
python3 code_smell_scanner.py src/ --verbose > smell_report.txt
```

---

## 📊 Output Format

### Summary Report
```
================================================================================
CODE SMELL ANALYSIS REPORT
================================================================================

Summary:
  Total code smells: 99
  Files with issues: 6

By Severity:
  🔴 Critical  : 1
  🟡 Major     : 9
  🔵 Minor     : 89

By Type:
  • Magic Number             : 89
  • Dead Code                : 5
  • Duplicate Code           : 4
  • God Class                : 1

Top Recommendations:
  🔴 CRITICAL: Address Large/God classes immediately
  🟡 MAJOR: Eliminate duplicate code through abstraction
  🔵 MINOR: Replace magic numbers with named constants
```

### Detailed Report (--verbose)
```
MoreCode_Anonymized/RGTData.cpp
────────────────────────────────────────────────────────────────────────────────

🟡 Long Method [Major] at line 150:
  Method 'processRecords' is 85 lines long (max: 50)
  💡 Break down into smaller methods (Extract Method refactoring)
  → Lines 150-235

🔴 God Class [Critical] at line 45:
  Class 'DataProcessor' has 25 methods and 18 fields (too many responsibilities)
  💡 Apply Single Responsibility Principle - extract related methods
  → Methods: 25, Fields: 18
```

---

## ⚙️ Configuration

Default thresholds (can be modified in code):

| Smell Type | Threshold |
|------------|-----------|
| Long Method | 50 lines |
| Long Parameter List | 5 parameters |
| Deep Nesting | 4 levels |
| Large Class | 500 lines |
| God Class | 20 methods, 15 fields |
| Complex Conditional | 3 logical operators |
| Duplicate Code | 5 similar lines |

---

## 🎯 Severity Levels

### 🔴 CRITICAL
Serious design flaws that should be addressed immediately
- Large Class (>500 lines)
- God Class (too many responsibilities)

### 🟡 MAJOR
Significant maintainability issues
- Long Method
- Long Parameter List
- Deep Nesting
- Dead Code
- Complex Conditional
- Duplicate Code

### 🔵 MINOR
Quality improvements
- Magic Number
- Data Clump

### ℹ️ INFO
Suggestions for improvement
- (Reserved for future enhancements)

---

## 💡 Refactoring Strategies

### Extract Method
**For:** Long methods, duplicate code
```cpp
// Before
void processOrder() {
    // 100 lines of code
}

// After
void processOrder() {
    validateOrder();
    calculateTotal();
    applyDiscounts();
    processPayment();
    sendConfirmation();
}
```

### Introduce Parameter Object
**For:** Long parameter lists, data clumps
```cpp
// Before
void createWindow(int x, int y, int width, int height,
                  string title, bool resizable, bool visible);

// After
struct WindowConfig {
    Rectangle bounds;
    string title;
    bool resizable, visible;
};
void createWindow(const WindowConfig& config);
```

### Replace Conditional with Polymorphism
**For:** Complex conditionals
```cpp
// Before
if (type == CIRCLE) {
    drawCircle();
} else if (type == RECTANGLE) {
    drawRectangle();
} else if (type == TRIANGLE) {
    drawTriangle();
}

// After
class Shape {
    virtual void draw() = 0;
};
shape->draw();  // Polymorphic call
```

### Extract Class
**For:** Large/God classes
```cpp
// Before
class User {
    // User data
    // Database methods
    // Email methods
    // Validation methods
};

// After
class User {
    UserData data;
    UserValidator validator;
    EmailService emailer;
};
```

---

## 📈 Interpreting Results

### High Smell Count
**If you see 100+ smells:**
- Don't panic! This is common in legacy code
- Prioritize by severity (Critical → Major → Minor)
- Tackle one smell type at a time
- Set realistic refactoring goals

### Low Smell Count
**If you see <10 smells:**
- Great! Code is relatively clean
- Address remaining issues before they grow
- Continue good coding practices

### Pattern Recognition
**If you see many of the same smell:**
- **Many Long Methods:** Need better decomposition habits
- **Many Magic Numbers:** Missing constant definitions
- **Many Duplicate Codes:** Missing abstractions
- **God Classes:** Over-centralized design

---

## 🔍 Comparison with MISRA Checker

| Feature | MISRA Checker | Code Smell Scanner |
|---------|---------------|---------------------|
| **Focus** | Safety, compliance | Design, maintainability |
| **Rules** | 169 MISRA rules | 10 code smells |
| **Severity** | Required/Advisory | Critical/Major/Minor |
| **Use Case** | Safety-critical code | All C++ projects |
| **Certification** | MISRA compliance | Code quality |

**Recommendation:** Use both tools together!
- MISRA Checker for safety compliance
- Code Smell Scanner for design quality

---

## 🎓 Best Practices

### When to Refactor
✅ **Good times:**
- Before adding new features
- During bug fixes (Boy Scout Rule)
- Dedicated refactoring sprints
- When smell impacts current work

❌ **Bad times:**
- During tight deadlines
- Without tests
- Without understanding the code
- Just because a tool says so

### Refactoring Safely
1. ✅ **Write tests first** (if they don't exist)
2. ✅ **Refactor in small steps**
3. ✅ **Run tests after each change**
4. ✅ **Commit frequently**
5. ✅ **Get code review**

### Prioritization
1. 🔴 **Critical smells** (Large/God classes)
2. 🟡 **Major smells affecting current work**
3. 🟡 **Major smells in frequently changed code**
4. 🔵 **Minor smells during maintenance**

---

## 📚 Further Reading

### Books
- **"Refactoring" by Martin Fowler** - The definitive guide
- **"Clean Code" by Robert C. Martin** - Best practices
- **"Working Effectively with Legacy Code" by Michael Feathers**

### Online Resources
- [Refactoring Guru](https://refactoring.guru) - Code smells catalog
- [Source Making](https://sourcemaking.com/refactoring) - Refactoring patterns

---

## 🚦 Quick Start Examples

### Example 1: Find Critical Issues
```bash
python3 code_smell_scanner.py src/ | grep "Critical"
```

### Example 2: Generate Full Report
```bash
python3 code_smell_scanner.py src/ --verbose > code_quality_report.txt
```

### Example 3: Check Single File Before Commit
```bash
python3 code_smell_scanner.py MyClass.cpp --verbose
```

### Example 4: Weekly Code Quality Check
```bash
# In CI/CD pipeline
python3 code_smell_scanner.py src/ > weekly_smell_report.txt
```

---

## 🎯 Success Metrics

**Track these over time:**
- Total smell count (should decrease)
- Critical smell count (should be 0)
- Smell density (smells per 1000 lines)
- Files with smells (percentage)

**Example tracking:**
```
Week 1:  99 smells (6 files, 1 critical)
Week 2:  85 smells (5 files, 0 critical) ✅
Week 3:  72 smells (4 files, 0 critical) ✅
Week 4:  58 smells (3 files, 0 critical) ✅
```

---

## ✅ Limitations

**What it CANNOT detect:**
- Architectural issues (layering violations)
- Performance problems
- Security vulnerabilities
- Thread safety issues
- Memory leaks
- Logic errors

**What it CAN detect:**
- Surface-level design problems
- Code complexity issues
- Maintainability concerns
- Common refactoring opportunities

---

## 🔧 Customization

To adjust thresholds, edit the scanner:

```python
# In code_smell_scanner.py
self.detectors = [
    LongMethodDetector(max_lines=50),      # Change to 75 for longer methods
    LongParameterListDetector(max_params=5), # Change to 7 for more params
    DeepNestingDetector(max_depth=4),      # Change to 5 for deeper nesting
    # ... etc
]
```

---

## 📊 Example Session

```bash
$ python3 code_smell_scanner.py MoreCode_Anonymized/

Scanning 6 C++ files...

================================================================================
CODE SMELL ANALYSIS REPORT
================================================================================

Summary:
  Total code smells: 99
  Files with issues: 6

By Severity:
  🔴 Critical  : 1
  🟡 Major     : 9
  🔵 Minor     : 89

By Type:
  • Magic Number             : 89
  • Dead Code                : 5
  • Duplicate Code           : 4
  • God Class                : 1

Top Recommendations:
  🔴 CRITICAL: Address Large/God classes immediately
  🟡 MAJOR: Eliminate duplicate code through abstraction
  🔵 MINOR: Replace magic numbers with named constants
```

---

## 🎉 Conclusion

The Code Smell Scanner helps you:
- ✅ Identify design problems early
- ✅ Improve code maintainability
- ✅ Reduce technical debt
- ✅ Make refactoring decisions easier
- ✅ Track code quality over time

**Remember:** Code smells are warnings, not errors. Use your judgment to decide what to refactor and when!

---

**Version:** 1.0.0
**Status:** ✅ Ready for use
**Compatibility:** Works with all C++ code (C++11 through C++23)
**Performance:** Fast pattern-based detection
