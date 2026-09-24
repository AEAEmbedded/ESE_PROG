# ESE_PROG - Programming 5 / 6 (2026-2027)

Course repository for **Programming 5 (Prog 5)** and **Programming 6 (Prog 6)** of the
Embedded Systems Engineering track, Software Engineering, HAN University of Applied Sciences.

| Course | Period | When | Folder |
| --- | --- | --- | --- |
| **Prog 5** | Period 1 | September - October 2026 | [`Prog5/`](Prog5/) - lectures, labs, assignments, example code |
| **Prog 6** | Period 2 | November 2026 - January 2027 | [`Prog6/`](Prog6/) - workshops, Hackathon, Challenge, rubrics |

## Welcome to Prog 5

Prog 5 and Prog 6 are the final lap of the programming courses in your ESE journey. Together they
work towards one overall goal: **becoming a professional embedded software and systems developer**.

Prog 5 (period 1) is about object-oriented C++ on microcontrollers (Arduino / SAMD21) and on the
Raspberry Pi: writing sensor libraries, designing them with UML, connecting them to the outside
world (I2C, MQTT) and testing them. Prog 6 (period 2) builds on that with software-engineering
practice - build systems, unit testing, clean code and design patterns - and applies it in the
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
tools. Being fluent in them is part of being a professional - see
[Coder vs Developer vs Software Engineer: what's the difference?](https://youtu.be/fcjBfSiyI0k)

### Handing in your work

Put your assignments in a (private) Git repository and add your lecturer as a collaborator
(GitHub username **jakorten**). Commit as you go - a
repository with a real history of small, meaningful commits is part of what is assessed.

## Prog 5 schedule and deadlines (period 1, 2026-2027)

The course starts on **Thursday 3 September 2026** and runs for seven weeks. Every week combines
one design principle, a C++ topic and a UML topic, and every Friday there is a lab. The first two
labs are Git and Requirements; from week 3 onwards the Friday lab is the next step of **one sensor
library** that grows week by week (the BME280 is the concrete sensor we use; another I2C sensor
works just as well). The same seven-step plan is on the schedule slide of every lecture deck.

| Week | Lecture (Thu) | Design principle | C++ | UML | Slides | Lab (Fri) |
| :-: | --- | --- | --- | --- | --- | --- |
| 1 | 3 Sep 2026 | Single Responsibility | Scope, namespaces, string | Use cases / class diagram | [0. Course introduction](Prog5/Keynotes/), [1. Introduction](Prog5/Keynotes/) | Fri 4 Sep: [Git workshop 1](Prog5/Workshops/Git/Workshop1_Basics.md) - the basics (clone, add, commit, push, .gitignore, handing in) |
| 2 | 10 Sep 2026 | Open-Closed Principle | Constructors, iterators, lambdas | Inheritance / generalization | [2. Constructors and more](Prog5/Keynotes/) | Fri 11 Sep: Requirements (SMART, FURPS+, MoSCoW, use cases) |
| 3 | 17 Sep 2026 | Liskov Substitution Principle | Lists, inline functions, default params | Activity diagrams | [3. Lists, inline functions and default params](Prog5/Keynotes/) | Fri 18 Sep: sensor library 1 - core, `Bus`, Arduino |
| 4 | 24 Sep 2026 | Interface Segregation Principle | Interfaces and abstract classes | Dependencies | [4. Interfaces and abstract classes](Prog5/Keynotes/) | Fri 25 Sep: sensor library 2 - Raspberry Pi, `Bus` / `Clock` split |
| 5 | 1 Oct 2026 | Dependency Inversion Principle | Threads, callbacks | Sequence diagrams | [5. Threading and callbacks](Prog5/Keynotes/) | Fri 2 Oct: sensor library 3 - sampler thread, callback, MQTT |
| 6 | 8 Oct 2026 | Coupling and cohesion | Polymorphism | Composition, packages | [6. Polymorphism](Prog5/Keynotes/) | Fri 9 Oct: sensor library 4 - polymorphism, UML design |
| 7 | 15 Oct 2026 | Embedded SOLID | Lifecycle: constructors, destructors, const | - | [7. Lifecycle](Prog5/Keynotes/) | Fri 16 Oct: sensor library 5 - RAII, host unit tests, refactor |

Each assignment is introduced in the lecture of the week listed below and is due **one week
later, on the Thursday of the following week**. Full hand-outs: [`Prog5/Assignments/`](Prog5/Assignments/).

| Week | Lecture | Assignment | Deadline |
| :-: | --- | --- | --- |
| 1 | Thu 3 Sep 2026 | **1.** Make a basic stepper motor library and hand it in via your Git repository. | Thu 10 Sep 2026 |
| 2 | Thu 10 Sep 2026 | **2.** Create a UML use-case diagram with description for a sensor application (e.g. KNMI / weather station). | Thu 17 Sep 2026 |
| 3 | Thu 17 Sep 2026 | **3.** [Sensor library, core + Arduino](Prog5/Assignments/week3/bme280_arduino_library.md): Bosch driver wrapped in a class, `Bus` interface, `ArduinoI2cBus`, example sketch. Tag `v0.3-arduino`. | Thu 24 Sep 2026 |
| 4 | Thu 24 Sep 2026 | **4.** [Same library on the Raspberry Pi](Prog5/Assignments/week4/bme280_raspberry_pi.md): CMake build, `LinuxI2cBus`, `Bus` / `Clock` split (ISP), dependency diagram. Tag `v0.4-pi`. | Thu 1 Oct 2026 |
| 5 | Thu 1 Oct 2026 | **5.** [Sampling thread, callback, MQTT](Prog5/Assignments/week5/bme280_mqtt.md): `Sampler`, `Publisher` / `MqttPublisher`, `EnvironmentSensor` + `FakeSensor`, sequence diagram sketch. Tag `v0.5-mqtt`. | Thu 8 Oct 2026 |
| 6 | Thu 8 Oct 2026 | **6.** [Design on paper](Prog5/Assignments/week6/bme280_uml_design.md): class, package and sequence diagrams of the library; `vector<EnvironmentSensor*>` with a second sensor. Tag `v0.6-design`. | Thu 15 Oct 2026 |
| 7 | Thu 15 Oct 2026 | **7.** [Unit tests on the host](Prog5/Assignments/week7/bme280_tests_and_refactor.md) with the `MockBus` (no mocking framework yet). <br> **8.** Review the library against DRY, KISS, SOLID, coupling / cohesion and the lifecycle checklist; add a second sensor and move the shared abstractions (`Bus`, `Clock`, `EnvironmentSensor`) into their own package; refactor and report. Tag `v1.0`. | Thu 22 Oct 2026 |

**Resit:** assignments that were not handed in on time, or that did not pass, can be (re)submitted
until the resit deadline of **Thursday 29 October 2026**.

Assignments 3-8 all build on the same sensor library (the Friday labs are its steps), so keep it in one repository and let its
history show how it grows: from a first Arduino version to a tested, refactored Pi library that
publishes over MQTT. Starter code per week is in [`Prog5/Code/`](Prog5/Code/) (`week2`: the
wrapper library, `week4`: Linux I2C bus, `week5`: sampler + MQTT).

## Learning outcomes

The learning outcomes below follow the HAN engineering competencies used in the rubrics
(*Analyze - Design - Manage - Control - Realize - Professionalize*). Prog 5 lays the foundation;
Prog 6 raises every outcome to a professional level and assesses it in the Hackathon and the
Challenge.

### Prog 5 - after period 1 you can

| # | Learning outcome | Competency |
| :-: | --- | --- |
| P5.1 | **Analyse** a realistic problem into user stories and SMART requirements, categorised with FURPS+ and prioritised with MoSCoW. | Analyze |
| P5.2 | **Design** software with UML: a use-case diagram with descriptions, a class diagram and a sequence diagram (and a state diagram where behaviour asks for it). | Design |
| P5.3 | **Implement** object-oriented C++ on embedded targets (Arduino / SAMD21 and Raspberry Pi): classes and encapsulation, constructors/destructors and object lifecycle, inheritance, abstract classes and interfaces, polymorphism, smart pointers, lambdas and callbacks, basic concurrency. | Realize |
| P5.4 | **Build reusable sensor libraries**: an Arduino library for an (I2C) sensor, a C++ library for an I2C sensor on the Raspberry Pi, and connect it to an MQTT broker. | Realize |
| P5.5 | **Apply DRY, KISS, SOLID and loose coupling / strong cohesion**, recognise where your own code violates them, refactor it and explain what improved. | Realize |
| P5.6 | **Test** a library with basic unit tests in C++. | Control |
| P5.7 | **Use Git professionally** for all your work: meaningful commits, branches, pull requests and a readable history, in a repository you share with your lecturer. | Manage |

### Prog 6 - after period 2 you can

| # | Learning outcome | Competency |
| :-: | --- | --- |
| P6.1 | **Set up and maintain a CMake build** for a C/C++ project with its own libraries and externally fetched test dependencies, and explain why build systems exist. | Realize |
| P6.2 | **Unit test at a professional level** with CppUTest, including test doubles and mocks for hardware dependencies, and position tests in the testing pyramid. | Control |
| P6.3 | **Write clean, well-commented, maintainable code**: recognise code smells, apply MISRA C++:2023 guidelines and use static-analysis tooling to check your own code. | Realize |
| P6.4 | **Apply embedded design patterns and architecture** (state, observer, hardware proxy, cyclic executive, debouncing, fixed-point arithmetic, humble object) with awareness of their run-time and memory overhead. | Design |
| P6.5 | **Realise a working proof-of-concept** for a healthcare case (Wilhelmina Children's Hospital, WKZ) - in a team under time pressure in the Hackathon and in the longer Challenge project - from analysis and SOLID design, through technical designs (class, sequence, state), to a tested implementation. | Analyze / Design / Realize |
| P6.6 | **Manage code as a team** in Git with commits, pull requests, blame and code reviews throughout the project. | Manage |
| P6.7 | **Reflect and advise**: give substantiated advice based on your analysis and proof-of-concept and present it convincingly (presentation, report or video), communicating professionally with the team, lecturers and domain experts. | Professionalize |

Assessment: Prog 5 by the weekly assignments (see schedule below); Prog 6 with the
[Hackathon and Prog 6 rubrics](Prog6/Rubrics/).

## Repository layout

| Folder | Contents |
| --- | --- |
| [`Prog5/Keynotes/`](Prog5/Keynotes/) | Lecture slides (PDF): course introduction and lectures 1-7. |
| [`Prog5/Labs/`](Prog5/Labs/) | Lab hand-outs (Lab 2: Arduino basics with OOP / SOLID; Lab 3: I2C sensor library). |
| [`Prog5/Assignments/`](Prog5/Assignments/) | Assignment hand-outs per week; from week 3 on all steps of the one sensor library (BME280 as the worked example). |
| [`Prog5/Code/week1/`](Prog5/Code/week1/) | Week 1 example code: stepper motor examples, `SimpleStepper` library, TB6600 test. |
| [`Prog5/Code/week2/`](Prog5/Code/week2/) | BME280 starter library: `Bus` interface, wrapper around the vendored Bosch BME280 SensorAPI, `MockBus` + doctest. |
| [`Prog5/Code/week4/`](Prog5/Code/week4/) | `rpi_platform`: `LinuxI2cBus` on `/dev/i2c-1` and a smoke test, CMake build of core + platform. |
| [`Prog5/Code/week5/`](Prog5/Code/week5/) | `mqtt_station`: `EnvironmentSensor`, `Sampler` (thread + callback), `Publisher` / `MqttPublisher` (libmosquitto), composition root. |
| [`Prog5/Code/Examples/`](Prog5/Code/Examples/) | Demo code used in the lectures and labs (abstract interfaces, object lifecycle, ToF sensor library, SOLID terminal, I2C scanner). |
| [`Prog5/Workshops/Git/`](Prog5/Workshops/Git/) | Git workshops 1 (the basics) and 2 (working together), with an interactive version. |
| [`Prog5/Demos/`](Prog5/Demos/) | Hand-outs: Git manual, CMake basics, interfaces demo. |
| [`Prog6/Workshops/`](Prog6/Workshops/) | Workshops: CMake, Unit Testing (CppUTest), Commenting, Clean Code / MISRA C++, Design Patterns & Architecture. |
| [`Prog6/Hackathon/`](Prog6/Hackathon/) | Hackathon material: the VitalSignsBox modules (ECG lead detection, SpO2 detection, temperature sensors) with firmware, API descriptions and hardware files. |
| [`Prog6/Challenge/`](Prog6/Challenge/) | Challenge project: assignment document and hardware design files (SensorHub, ActuatorHub, sim modules, adapters). |
| [`Prog6/Rubrics/`](Prog6/Rubrics/) | Assessment rubrics 2026-2027 (Prog 6 v1.5, Hackathon v1.4; Markdown source + PDF) and previous editions. |
| [`Resources/`](Resources/) | Background reading, e.g. choosing a development method. |

## How to use

1. Clone the repository: `git clone https://github.com/AEAEmbedded/ESE_PROG`
2. Arduino sketches (`*.ino`): open the sketch folder in the Arduino IDE and upload to your board.
3. CMake projects (workshops): configure out-of-source, e.g.
   ```bash
   cmake -S Prog6/Workshops/UnitTesting/Source/TemperatureTestFinished -B build
   cmake --build build
   ctest --test-dir build
   ```
   Build directories are ignored by git - do not commit them.
4. Refer to the keynotes, labs and workshop READMEs for instructions.

## Related repositories

* **[PiTest](https://github.com/jakorten/PiTest)** - Raspberry Pi 4 to SensorHub (SAMD21) I2C communication test (Python for the Pi, C++ Arduino sketches for the SAMD21).

## Disclaimer

This repository is for educational purposes. The code may be incomplete or contain errors. Use at your own risk.

## Contact

Johan Korten - johan.korten@han.nl
