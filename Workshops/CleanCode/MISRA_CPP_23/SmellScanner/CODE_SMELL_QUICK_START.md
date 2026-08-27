# Code Smell Scanner - Quick Start

**Fast track to detecting code quality issues in your C++ code**

---

## 🚀 30-Second Quick Start

```bash
# Scan a single file
python3 code_smell_scanner.py MyClass.cpp

# Scan entire directory
python3 code_smell_scanner.py src/

# Detailed report with all issues
python3 code_smell_scanner.py src/ -v

# Extra details with code snippets
python3 code_smell_scanner.py src/ -vv
```

---

## 📊 Verbosity Levels

### Level 0: Summary Only (default)
```bash
python3 code_smell_scanner.py MoreCode/
```

**Shows:**
- Total smell count
- Breakdown by severity
- Breakdown by type (with percentage bars)
- Top recommendations

**Example output:**
```
Summary:
  Total code smells: 99
  Files with issues: 6

By Severity:
  🔴 Critical  :   1 (  1.0%)
  🟡 Major     :   9 (  9.1%)
  🔵 Minor     :  89 ( 89.9%)

By Type:
  • Magic Number             :  89 ( 89.9%) █████████████████
  • Dead Code                :   5 (  5.1%) █
  • Duplicate Code           :   4 (  4.0%)
  • God Class                :   1 (  1.0%)
```

---

### Level 1: Verbose (`-v`)
```bash
python3 code_smell_scanner.py MoreCode/ -v
```

**Shows everything from Level 0, plus:**
- File breakdown (which files have most smells)
- Detailed list of each smell
- Statistics (average smells per file, most problematic file)
- Code quality score

**Example additions:**
```
Files Analyzed:
  🔴 algdef.cpp                              :  32 smells
  🔴 RGTData.h                               :  29 smells
  🟡 RGTData.cpp                             :  13 smells

Statistics:
  Average smells per file: 16.5
  Most problematic file: algdef.cpp
  Most common smell: Magic Number
  Code quality score: 67.0/100
```

---

### Level 2: Extra Verbose (`-vv`)
```bash
python3 code_smell_scanner.py MoreCode/ -vv
```

**Shows everything from Level 1, plus:**
- Code snippets showing context (±3 lines)
- Grouped smells by severity per file
- Detailed refactoring priorities

**Example additions:**
```
🟡 Dead Code [Major] at line 95:
  Code after return statement is unreachable
  💡 Remove unreachable code
  → else
      Code context:
        92     {
        93         if (this == &rhs)
        94             return(false);
        95   →     else              ← SMELL HERE
        96         {
        97             if ((tel != rhs.teller()) || (noem != rhs.noemer()))
        98                 return(true);

Refactoring Priority:
  1. Start with Critical smells (they have the biggest impact)
  2. Then tackle Major smells in frequently-changed files
  3. Address Minor smells during routine maintenance
  4. Use 'Boy Scout Rule': Leave code better than you found it
```

---

## 🎯 Common Use Cases

### 1. Quick Health Check
**Goal:** Get overall code quality at a glance

```bash
python3 code_smell_scanner.py src/
```

**Look for:** Critical/Major smell counts

---

### 2. Pre-Commit Check
**Goal:** Check your changes before committing

```bash
python3 code_smell_scanner.py MyChangedFile.cpp -v
```

**Look for:** Any new smells you introduced

---

### 3. Code Review Preparation
**Goal:** Detailed report for code review

```bash
python3 code_smell_scanner.py feature_branch/ -vv > review_report.txt
```

**Look for:** Major issues to address before review

---

### 4. Refactoring Planning
**Goal:** Identify refactoring targets

```bash
# Find files with most issues
python3 code_smell_scanner.py src/ -v | grep "🔴"
```

**Look for:** Files with >20 smells (marked 🔴)

---

### 5. Track Progress Over Time
**Goal:** Measure code quality improvements

```bash
# Week 1
python3 code_smell_scanner.py src/ > week1_report.txt

# Week 2 (after refactoring)
python3 code_smell_scanner.py src/ > week2_report.txt

# Compare
diff week1_report.txt week2_report.txt
```

**Look for:** Decreasing total smell count

---

## 🔍 Interpreting Results

### Severity Icons

| Icon | Severity | Action Required |
|------|----------|-----------------|
| 🔴 | **Critical** | Fix immediately! (Large/God classes) |
| 🟡 | **Major** | Fix soon (Long methods, duplicates) |
| 🔵 | **Minor** | Fix during maintenance (Magic numbers) |

### File Markers (in `-v` mode)

| Marker | Smell Count | Health |
|--------|-------------|--------|
| 🔴 | >20 smells | Needs urgent refactoring |
| 🟡 | 10-20 smells | Needs attention |
| 🔵 | <10 smells | Relatively healthy |

### Code Quality Score (in `-v` mode)

| Score | Grade | Meaning |
|-------|-------|---------|
| 80-100 | 🟢 Good | Well-maintained code |
| 60-79 | 🟡 Fair | Some refactoring needed |
| 0-59 | 🔴 Poor | Significant technical debt |

---

## 🎓 Top 5 Most Common Smells

### 1. Magic Number (🔵 Minor)
**What it is:** Unexplained numeric literals
```cpp
if (speed > 299792458) { }  // What is 299792458?
```

**Fix:**
```cpp
constexpr int SPEED_OF_LIGHT = 299792458;
if (speed > SPEED_OF_LIGHT) { }
```

---

### 2. Dead Code (🟡 Major)
**What it is:** Unreachable or commented code
```cpp
return value;
doSomething();  // Never runs!
```

**Fix:**
```cpp
return value;
// doSomething() removed
```

---

### 3. Long Method (🟡 Major)
**What it is:** Methods >50 lines
```cpp
void processData() {
    // 80 lines...
}
```

**Fix:**
```cpp
void processData() {
    validateInput();
    transformData();
    saveResults();
}
```

---

### 4. Duplicate Code (🟡 Major)
**What it is:** Same code repeated
```cpp
// Function 1
validate();
transform();
save();

// Function 2
validate();  // Duplicate!
transform(); // Duplicate!
save();      // Duplicate!
```

**Fix:**
```cpp
void processData() {
    validate();
    transform();
    save();
}
// Both functions call processData()
```

---

### 5. God Class (🔴 Critical)
**What it is:** Classes doing too much
```cpp
class Application {
    // 30 methods, 25 fields
    // Does: DB, UI, network, logging...
};
```

**Fix:**
```cpp
class Application {
    DatabaseManager db;
    UIController ui;
    NetworkClient net;
    Logger log;
};
```

---

## 💡 Quick Tips

### For Best Results

✅ **DO:**
- Run regularly (e.g., weekly)
- Focus on Critical/Major first
- Refactor in small steps
- Track progress over time

❌ **DON'T:**
- Try to fix everything at once
- Ignore Critical smells
- Refactor without tests
- Blindly follow all suggestions

### Priority Guide

1. **Critical smells** → Fix ASAP
2. **Major smells in frequently changed files** → Fix next sprint
3. **Major smells in stable files** → Plan refactoring
4. **Minor smells** → Fix during routine maintenance

---

## 📊 Example Session

```bash
$ python3 code_smell_scanner.py src/ -v

Scanning 6 C++ files...

Summary:
  Total code smells: 99
  Files with issues: 6

Files Analyzed:
  🔴 algdef.cpp        :  32 smells  ← Start here!
  🔴 RGTData.h         :  29 smells
  🟡 RGTData.cpp       :  13 smells
  🔵 algdef.h          :   9 smells
  🔵 dataPakket.cpp    :   8 smells
  🔵 dataPakket.h      :   8 smells

By Severity:
  🔴 Critical  :   1 (  1.0%)  ← Fix first!
  🟡 Major     :   9 (  9.1%)  ← Fix next
  🔵 Minor     :  89 ( 89.9%)  ← Fix later

Statistics:
  Code quality score: 67.0/100  ← Fair

Top Recommendations:
  🔴 CRITICAL: Address Large/God classes immediately
  🟡 MAJOR: Eliminate duplicate code
  🔵 MINOR: Replace magic numbers
```

**Action Plan:**
1. Fix the 1 God Class ← Biggest impact
2. Remove 5 dead code instances
3. Extract 4 duplicate code sections
4. Replace 89 magic numbers (gradually)

---

## 🔧 Command Reference

| Command | Output | Use When |
|---------|--------|----------|
| `python3 code_smell_scanner.py file.cpp` | Summary | Quick check |
| `python3 code_smell_scanner.py file.cpp -v` | Detailed | Deep dive |
| `python3 code_smell_scanner.py file.cpp -vv` | Extra | Refactoring |
| `python3 code_smell_scanner.py dir/` | Summary | Directory scan |
| `python3 code_smell_scanner.py dir/ > report.txt` | File | Save results |

---

## ✅ Success Example

**Before refactoring:**
```
Total code smells: 99
  🔴 Critical: 1
  🟡 Major: 9
  🔵 Minor: 89
Code quality: 67.0/100
```

**After 2 weeks:**
```
Total code smells: 45  ← 54 fewer!
  🔴 Critical: 0  ← Fixed!
  🟡 Major: 3     ← 6 fewer!
  🔵 Minor: 42    ← 47 fewer!
Code quality: 85.0/100 ← Much better!
```

---

## 🎯 Next Steps

1. **Run first scan:** `python3 code_smell_scanner.py src/ -v`
2. **Identify critical issues:** Look for 🔴 markers
3. **Create refactoring plan:** Focus on worst files
4. **Track progress:** Run weekly scans
5. **Celebrate improvements:** Watch quality score rise!

---

**For more details, see:** `CODE_SMELL_SCANNER_GUIDE.md`

---

**Remember:** Code smells aren't bugs—they're opportunities to improve!
