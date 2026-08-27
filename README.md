# ESE_PROG — Programming 5 / 6 (2026-2027)

Course repository for **Programming 5 (Prog5)** and **Programming 6 (Prog6)** of the
Embedded Systems Engineering track, Software Engineering, HAN University of Applied Sciences.

## Welcome to Prog 5

Prog 5 and Prog 6 are the final lap of the programming courses in your ESE journey. Together they
work towards one overall goal: **becoming a professional embedded software and systems developer**.

Prog 5 (period 1) is about object-oriented C++ on microcontrollers (Arduino / SAMD21) and on the
Raspberry Pi: writing sensor libraries, designing them with UML, connecting them to the outside
world (I2C, MQTT) and testing them. Prog 6 (period 2) builds on that with software-engineering
practice — build systems, unit testing, clean code and design patterns — and applies it in the
Hackathon and the Challenge project.

### Why everything is done with Git

All work in Prog 5 and 6 is done with **Git**, because it is the only way to reach our learning
goals. Git:

* lets several developers work on the same project at the same time;
* keeps a complete history of every change to the code;
* makes it easy to revert a change or recover an earlier version;
* helps you resolve conflicts between contributions efficiently.

More importantly, it takes you out of an artificial learning environment and into the professional
field: the companies where you will do your internship and, later, land a job use exactly these
tools. Being fluent in them is part of being a professional — see
[Coder vs Developer vs Software Engineer: what's the difference?](https://youtu.be/fcjBfSiyI0k)

### Handing in your work

Put your assignments in a (private) Git repository and add your lecturer as a collaborator
(GitHub username **jakorten**), or use our GitHub Classroom environment. Commit as you go — a
repository with a real history of small, meaningful commits is part of what is assessed.

## Prog 5 schedule and deadlines (period 1, 2026-2027)

The course starts on **Thursday 3 September 2026** and runs for seven weeks. Each assignment is
introduced in the lecture of the week listed below and is due **one week later, on the Thursday
of the following week**.

| Week | Lecture | Assignment | Deadline |
| :-: | --- | --- | --- |
| 1 | Thu 3 Sep 2026 | **1.** Make a basic stepper motor library and submit it via GitHub Classroom. | Thu 10 Sep 2026 |
| 2 | Thu 10 Sep 2026 | **2.** Create a UML use-case diagram with description for a sensor application (e.g. KNMI / weather station). | Thu 17 Sep 2026 |
| 3 | Thu 17 Sep 2026 | **3.** Create an Arduino library in C++ for a sensor (bring your own, or use the BMP280). | Thu 24 Sep 2026 |
| 4 | Thu 24 Sep 2026 | **4.** Create a C++ library for an I2C sensor or function on a Raspberry Pi. | Thu 1 Oct 2026 |
| 5 | Thu 1 Oct 2026 | **5.** Extend the Pi sensor library to connect and publish to an MQTT broker, in C++. | Thu 8 Oct 2026 |
| 6 | Thu 8 Oct 2026 | **6.** Create a UML class diagram and a sequence diagram for your sensor library. | Thu 15 Oct 2026 |
| 7 | Thu 15 Oct 2026 | **7.** Write unit tests for your Pi sensor library in C++ (basic — no mocking yet). <br> **8.** Review your sensor library against DRY, KISS, SOLID and loose coupling / strong cohesion; refactor it and report which principles you improved on. | Thu 22 Oct 2026 |

**Resit:** assignments that were not handed in on time, or that did not pass, can be (re)submitted
until the resit deadline of **Thursday 29 October 2026**.

Assignments 3–8 all build on the same sensor library, so keep it in one repository and let its
history show how it grows: from a first Arduino version to a tested, refactored Pi library that
publishes over MQTT.

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
