# Prog 5 - period 1 (September - October 2026)

Object-oriented C++ on embedded targets: from a first stepper-motor library to a designed,
tested and refactored sensor library on Arduino and Raspberry Pi. The schedule, deadlines and
learning outcomes are in the [main README](../README.md).

| Folder | Contents |
| --- | --- |
| [`Keynotes/`](Keynotes/) | Lecture slides - `0. Course introduction`, then lectures `1` - `7`: Introduction (single responsibility), Constructors and more, Lists / inline functions / default params, Interfaces and abstract classes, Threading and callbacks, Polymorphism, Lifecycle. |
| [`Labs/`](Labs/) | Lab hand-outs: Lab 2 (Arduino basics with OOP / SOLID), Lab 3 (I2C sensor library, with class diagram in [`Prog5_Lab3_I2CSensorLib.md`](Labs/Prog5_Lab3_I2CSensorLib.md)). |
| [`Assignments/`](Assignments/) | Assignment hand-outs per week ([overview](Assignments/README.md)). Week 2: use-case diagram; weeks 3-7: the BME280 library, one layer per week (Arduino, Pi, MQTT, UML design, tests + refactor). |
| [`Code/week1/`](Code/week1/) | Week 1: stepper motor examples `1a` - `1g`, the `SimpleStepper` Arduino library, a plain-C `stepper_library` and a TB6600 driver test. |
| [`Code/week2/`](Code/week2/) | BMP280/BME280 starter library: `Bus` interface, wrapper around the Bosch driver, `MockBus` + doctest. |
| [`Code/week4/`](Code/week4/) | `rpi_platform`: `LinuxI2cBus` + smoke test on the Raspberry Pi. |
| [`Code/week5/`](Code/week5/) | `mqtt_station`: sampler thread, callback, `Publisher` / `MqttPublisher`. |
| [`Code/Examples/`](Code/Examples/) | Lecture demo code: `AbstractInterfaces` (dogs & fish), `Lifecycle` (raw vs smart pointers), `LibDemoArduinoToF` (VL6180X ToF sensor + `I2CHelper`, used in Lab 3), `SOLIDTerminal`, `SHTExample`, `WireScanner`, `UnitTesting`, `PiStuff` (Pi Wi-Fi setup template). |
| [`Workshops/Git/`](Workshops/Git/) | Git workshops: 1. The basics (Fri 4 Sep: clone, add, commit, push, .gitignore, handing in) and 2. Working together (branches, pull requests, conflicts, undo). Markdown plus an interactive `index.html`. |
| [`Demos/`](Demos/) | Hand-outs: HAN SE Git manual, CMake basics, the DogDemo on interfaces. |
| [`Brightspace_Prog5_course_page.html`](Brightspace_Prog5_course_page.html) | Text of the Brightspace course page (welcome, learning outcomes, week planner) - paste into the Brightspace HTML editor. |
