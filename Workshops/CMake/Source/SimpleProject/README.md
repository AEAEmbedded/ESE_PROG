# SimpleProject - Step 1

**CMake Workshop - HAN University**
**The Starting Point: Single File Compilation**

---

## What This Is

This is the simplest possible C++ project - a single file that prints a message. This is **Step 1** of the CMake workshop, showing you the basics before things get complicated.

---

## Files

```
SimpleProject/
├── README.md           # This file
└── main.cpp           # Single source file
```

---

## What You'll Learn

- How to compile a single C++ file
- Basic g++ command syntax
- That simple projects don't need build systems... yet!

---

## How to Build

### Compile with g++

```bash
g++ main.cpp -o sensor_app
```

**Breakdown:**
- `g++` - The C++ compiler
- `main.cpp` - The source file to compile
- `-o sensor_app` - Output executable named "sensor_app"

### Run the Program

```bash
./sensor_app
```

**Expected Output:**
```
Reading SHT45 sensor...
```

---

## The Lesson

**This is easy!**

For a single file, manual compilation is simple. But what happens when you have 2 files? 10 files? 100 files?

That's why we need build systems. Move on to **Step 2: SimpleLibrary** to see complexity increase!

---

## Code Quality

Even this simple example follows:
- ✅ Clean Code principles
- ✅ Modern C++ (C++17)
- ✅ MISRA C++ guidelines
- ✅ Proper comments and structure

---

## Next Step

Continue to: `../SimpleLibrary/` (Step 2)

Or return to: `../../README.md` (Workshop overview)

---

**Enjoy the simplicity while it lasts!** 😊
