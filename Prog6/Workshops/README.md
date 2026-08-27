# Workshops Prog 6

Hands-on workshops for Programming 6 (period 2, 2026-2027). Each workshop folder contains the slides
(`Keynotes/`, as Keynote, PowerPoint and PDF) and the accompanying source code (`Source/` or
`CodeExamples/`).

| Workshop | Folder | Topic |
| --- | --- | --- |
| 1 | [`CMake/`](CMake/) | Why build systems exist, from Make to CMake, libraries, staged example project (`BiggerLibraryCMakeV2/stage1..6`). |
| 2 | [`UnitTesting/`](UnitTesting/) | Unit testing with CppUTest: minimal test, temperature-conversion example (start, complex, finished) and mocking. |
| 3 | [`Commenting/`](Commenting/) | Writing useful comments and documentation; exercises 1-5. |
| 4 | [`CleanCode/`](CleanCode/) | Clean code, code smells and MISRA C++:2023, including the `SmellScanner` tooling. |
| 5 | [`PatternsArchitecture/`](PatternsArchitecture/) | Design patterns for embedded systems (cyclic executive, debouncing, hardware proxy, observer, state, fixed-point) and vtable overhead visualisations. |

## Building the CMake examples

All C++ examples are plain CMake projects; test dependencies (CppUTest) are fetched with
`FetchContent`. Always build out-of-source and keep build directories out of git:

```bash
cmake -S UnitTesting/Source/TemperatureTestFinished -B build
cmake --build build
ctest --test-dir build --output-on-failure
```
