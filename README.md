# ESE_PROG — Programming 5 / 6 (2026-2027)

Course repository for **Programming 5 (Prog5)** and **Programming 6 (Prog6)** of the
Embedded Systems Engineering track, Software Engineering, HAN University of Applied Sciences.

Prog5 focuses on object-oriented C++ on microcontrollers (Arduino / SAMD21); Prog6 continues
with software-engineering practice (build systems, unit testing, clean code, design patterns)
applied in the Hackathon and the Challenge project.

## Repository layout

| Folder | Contents |
| --- | --- |
| `Keynotes/` | Lecture slides (PDF). `Legacy/` holds the previous slide set for topics not yet re-issued. |
| `Labs/` | Lab assignments (Lab 2: Arduino basics with OOP / SOLID; Lab 3: I2C sensor library). |
| `Assignments/` | Weekly assignments (e.g. week 2: UML use-case diagram for the syringe system). |
| `Code/202627/` | Weekly example code for this academic year (week 1: stepper motor examples, `SimpleStepper` library, TB6600 test). |
| `Code/Older/` | Older demo code referenced by the labs and keynotes (abstract interfaces, object lifecycle, ToF sensor library, SOLID terminal, I2C scanner). |
| `Demos/` | Demo hand-outs (Git manual, CMake basics, interfaces demo). |
| `Workshops/` | Prog6 workshops: CMake, Unit Testing (CppUTest), Commenting, Clean Code / MISRA C++, Design Patterns & Architecture. See [`Workshops/README.md`](Workshops/README.md). |
| `Hackaton2027/` | Hackathon material: the VitalSignsBox modules (ECG lead detection, SpO2 detection, temperature sensors) with firmware, API descriptions and hardware files. |
| `Challenge/` | Prog6 Challenge: assignment document and hardware design files (SensorHub, ActuatorHub, sim modules, adapters). |
| `Rubrics/` | Assessment rubrics: `202627/` (Prog 6 v1.5 and Hackathon v1.4, Markdown source + PDF) and `Legacy/` (previous editions). |
| `UsefulStuff/` | Background reading, e.g. choosing a development method. |

## How to use

1. Clone the repository: `git clone https://github.com/AEAEmbedded/ESE_PROG`
2. Arduino sketches (`*.ino`): open the sketch folder in the Arduino IDE and upload to your board.
3. CMake projects (workshops): configure out-of-source, e.g.
   ```bash
   cmake -S Workshops/UnitTesting/Source/TemperatureTestFinished -B build
   cmake --build build
   ctest --test-dir build
   ```
   Build directories are ignored by git — do not commit them.
4. Refer to the keynotes, labs and workshop READMEs for instructions.

## Key topics

Prog5
* Classes, constructors, inline functions, default parameters
* Single Responsibility and the other SOLID principles
* Interfaces and abstract classes, polymorphism
* Lists and more advanced data structures
* Threading and callbacks
* Object lifecycle and smart pointers

Labs
* Arduino basics using SOLID / OOP (blinking, timing)
* I2C communication and sensor interfacing
* MQTT and Raspberry Pi (more high-level focus)

Prog6
* Build systems with CMake
* Unit testing and mocking with CppUTest
* Commenting and documentation
* Clean code, code smells and MISRA C++:2023
* Design patterns and architecture for embedded systems

## Related repositories

* **[PiTest](https://github.com/jakorten/PiTest)** — Raspberry Pi 4 to SensorHub (SAMD21) I2C communication test (Python for the Pi, C++ Arduino sketches for the SAMD21).

## Disclaimer

This repository is for educational purposes. The code may be incomplete or contain errors. Use at your own risk.

## Contact

Johan Korten — johan.korten@han.nl
