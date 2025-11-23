# CMake Workshop Presentation

**HAN University of Applied Sciences**
**Software Engineering - Programming 6**
**Johan Korten - v1.0 2025**

---

## Presentation Files

This directory contains the workshop presentation in multiple formats:

| Format | File | Description | Use For |
|--------|------|-------------|---------|
| **Keynote** | `Workshop CMake 2025.key` | Original presentation (macOS) | Editing and presenting on Mac |
| **PDF** | `Workshop CMake 2025.pdf` | Portable Document Format | Viewing, printing, sharing |
| **PowerPoint** | `Workshop CMake 2025.pptx` | Microsoft PowerPoint format | Presenting on Windows/Linux |

---

## Workshop Topics Covered

This presentation accompanies the hands-on CMake workshop and covers:

### 1. Introduction
- What is CMake?
- Why do we need build systems?
- Workshop goals and structure

### 2. The Problem
- Manual compilation challenges
- Scaling issues with multiple files
- Platform portability problems
- The pain of long g++ commands

### 3. Traditional Makefiles
- How Makefiles work
- Benefits over manual compilation
- Limitations and pain points
- Why we need something better

### 4. CMake Solution
- Modern build system generator
- Cross-platform capabilities
- Automatic dependency management
- Integration with modern IDEs

### 5. Hands-On Examples
- Step-by-step progression:
  - SimpleProject (1 file)
  - SimpleLibrary (2 files)
  - BiggerLibrary (6 files, manual)
  - BiggerLibraryMake (with Makefile)
  - BiggerLibraryCMake (with CMake)

### 6. Code Quality
- MISRA C++ compliance
- Clean Code principles
- Bjarne Stroustrup guidelines
- RAII and modern C++ best practices

### 7. CMake Best Practices
- Out-of-source builds
- Project structure
- Common commands
- Troubleshooting tips

---

## Using the Presentation

### For Instructors

**Opening the Presentation:**
- **On macOS:** Double-click `Workshop CMake 2025.key` (requires Keynote)
- **On Windows/Linux:** Use `Workshop CMake 2025.pptx` (requires PowerPoint or LibreOffice)
- **For sharing:** Use `Workshop CMake 2025.pdf`

**Presentation Flow:**
1. Start with theory (slides 1-10)
2. Demonstrate SimpleProject (live coding)
3. Show the progression to BiggerLibrary
4. Demonstrate the pain of manual compilation
5. Show Makefile solution and its limitations
6. Reveal CMake as the elegant solution
7. Live demo of CMake build process
8. Hands-on exercises for students

**Tips:**
- Use live terminal demos alongside slides
- Emphasize the pain points before showing solutions
- Let students experience the manual compilation frustration
- Show real-time CMake configuration and build
- Demonstrate cross-platform capabilities if possible

### For Students

**Viewing Options:**
1. **PDF Version** - Best for:
   - Quick reference during hands-on work
   - Viewing on any device
   - Printing notes

2. **Following Along:**
   - Open PDF on second monitor
   - Work through examples in terminal
   - Reference README.md in Source/ directory

---

## Workshop Structure

The presentation supports this learning path:

```
Theory (Presentation)
    ↓
Demonstration (Instructor)
    ↓
Hands-On Practice (Students)
    ↓
Exercises & Discussion
```

---

## Key Messages

The presentation emphasizes these core concepts:

### 1. Build Systems Evolution
```
Manual Compilation
    ↓ (Better but not enough)
Makefiles
    ↓ (The modern solution)
CMake
```

### 2. The Pain-Solution Pattern
- **Show the pain first** - Manual compilation of 6 files
- **Partial solution** - Makefiles help but have issues
- **Elegant solution** - CMake solves everything

### 3. Real-World Relevance
- Professional projects use CMake
- Industry standard for C/C++ projects
- Essential for cross-platform development
- Required skill for modern C++ developers

### 4. Quality Code
- All examples follow MISRA C++
- Clean Code principles applied
- Modern C++ (C++17) features
- Bjarne Stroustrup guidelines

---

## Presentation Sections

### Opening (Slides 1-5)
- Workshop introduction
- Prerequisites check
- Learning objectives
- Workshop structure overview

### Problem Introduction (Slides 6-15)
- Single file compilation (easy)
- Multiple files (getting harder)
- Complex projects (painful!)
- The need for automation

### Makefiles (Slides 16-25)
- What are Makefiles?
- How they help
- Limitations and problems
- Platform-specific issues

### CMake Solution (Slides 26-40)
- What is CMake?
- How it works
- Key benefits
- Comparison with Makefiles

### Hands-On Guide (Slides 41-50)
- Step-by-step instructions
- Live demonstrations
- Common commands
- Troubleshooting

### Code Quality (Slides 51-55)
- MISRA compliance
- Clean Code
- Modern C++ practices
- RAII examples

### Exercises (Slides 56-60)
- Practice tasks
- Challenge problems
- Group activities
- Discussion points

### Summary (Slides 61-65)
- Key takeaways
- Why CMake matters
- Further learning resources
- Q&A

---

## Supporting Materials

This presentation works together with:

1. **Source Code** - `../Source/` directory
   - 5 progressive examples
   - Complete working code
   - Build scripts included

2. **README.md** - `../README.md`
   - Detailed instructions
   - Command reference
   - Troubleshooting guide

3. **Hands-On Exercises**
   - Add new source files
   - Modify CMakeLists.txt
   - Cross-platform builds
   - Debug vs Release configs

---

## Presentation Notes

### Timing Guide
- **Total Duration:** 120 minutes (2 hours)
- Introduction: 10 min
- Theory & Problems: 20 min
- Makefile Discussion: 15 min
- CMake Introduction: 20 min
- Live Demonstrations: 25 min
- Hands-On Practice: 25 min
- Exercises & Q&A: 15 min

### Interactive Elements
- Live coding demonstrations
- Terminal command execution
- Real-time error fixing
- Student participation exercises

### Equipment Needed
- Projector/Screen for slides
- Live coding setup (terminal visible)
- Students need laptops with:
  - CMake installed
  - C++ compiler (g++/clang/MSVC)
  - Text editor or IDE
  - Terminal access

---

## Customization

Instructors can customize the presentation for:

- **Different Experience Levels:**
  - Beginners: More explanation, slower pace
  - Advanced: Skip basics, focus on best practices

- **Time Constraints:**
  - Short version (60 min): Skip Makefile deep dive
  - Extended version (180 min): Add advanced CMake features

- **Platform Focus:**
  - Emphasize specific platforms (Linux/Windows/macOS)
  - Show platform-specific generators

---

## Version History

### v1.0 (2025)
- Initial release
- 5-step progressive examples
- MISRA C++ compliant code
- Modern C++17 features
- Comprehensive exercises

---

## Additional Resources

### For Instructors
- [CMake Official Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)
- [Modern CMake Guide](https://cliutils.gitlab.io/modern-cmake/)
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/)

### For Students
- Workshop source code in `../Source/`
- Main workshop README in `../README.md`
- CMake documentation: https://cmake.org/documentation/
- Practice exercises in workshop materials

---

## License & Usage

This presentation is part of the HAN University curriculum:
- **For HAN students:** Free to use for learning
- **For instructors:** Modify as needed for teaching
- **Attribution:** Please credit HAN University and Johan Korten

---

## Contact & Feedback

For questions, corrections, or suggestions:
- Contact your course instructor
- Check the main workshop README
- Consult CMake documentation

---

## Quick Reference

### File Formats Compared

| Feature | Keynote (.key) | PowerPoint (.pptx) | PDF (.pdf) |
|---------|---------------|-------------------|------------|
| Edit on Mac | ✅ Yes | ⚠️ Limited | ❌ No |
| Edit on Windows | ❌ No | ✅ Yes | ❌ No |
| View anywhere | ⚠️ Mac only | ⚠️ Needs Office | ✅ Yes |
| Animations | ✅ Full | ⚠️ Some lost | ❌ Static |
| File size | Medium | Medium | Small |
| Best for | Presenting (Mac) | Presenting (Win) | Viewing/Printing |

### Recommended Usage

- **Teaching:** Use .key (Mac) or .pptx (Windows)
- **Students:** Use .pdf for reference
- **Sharing:** Use .pdf
- **Editing:** Use .key (original) or .pptx

---

**Enjoy the workshop!** 🎓

---

*Part of the Software Engineering curriculum at HAN University of Applied Sciences. Builds on Clean Code and MISRA C++ workshops.*
