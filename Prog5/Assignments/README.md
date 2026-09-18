# Prog 5 assignments - one sensor library, seven weeks

From week 3 onwards every assignment adds one layer to the **same sensor library**, and the Friday
lab of that week is where you build that layer. The BME280 is the sensor we use as the concrete
example; a BMP280 or another I2C sensor works just as well, the structure is the same. Keep it in **one Git repository**, commit as you
go and tag each hand-in (`v0.3-arduino`, `v0.4-pi`, ...). The history of that repository is part of
what is assessed.

| Week | Lecture (Thu) | What the library gains (= Friday lab) | Principle / C++ / UML it practises | Hand-out | Deadline |
| :-: | --- | --- | --- | --- | --- |
| 1 | 3 Sep | - (stepper motor warm-up) | SRP, scope / namespaces, class diagram | [`week1`](../Code/week1/Assignment/student_instructions.md) | Thu 10 Sep |
| 2 | 10 Sep | - (use-case diagram of the weather station it will live in) | OCP, constructors, use cases | [`week2`](week2/syringe_system_requirements.md) | Thu 17 Sep |
| 3 | 17 Sep | **Core + Arduino**: Bosch driver wrapped in a class, `Bus` interface, `ArduinoI2cBus` | LSP, default params, activity diagram | [`week3`](week3/bme280_arduino_library.md) | Thu 24 Sep |
| 4 | 24 Sep | **Second platform**: `LinuxI2cBus` on the Raspberry Pi, `Bus`/`Clock` split, CMake | ISP, abstract classes, dependencies | [`week4`](week4/bme280_raspberry_pi.md) | Thu 1 Oct |
| 5 | 1 Oct | **Out into the world**: sampling thread, measurement callback, `Publisher` -> MQTT | DIP, threads / callbacks, sequence diagram | [`week5`](week5/bme280_mqtt.md) | Thu 8 Oct |
| 6 | 8 Oct | **Design on paper**: class diagram, package diagram, sequence diagram; `EnvironmentSensor` polymorphism | coupling / cohesion, polymorphism, composition / packages | [`week6`](week6/bme280_uml_design.md) | Thu 15 Oct |
| 7 | 15 Oct | **Trustworthy**: host unit tests with the `MockBus`, RAII review, refactor report | Embedded SOLID, lifecycle / const | [`week7`](week7/bme280_tests_and_refactor.md) | Thu 22 Oct |

Resit deadline for everything: **Thursday 29 October 2026**.

## Hand-in rules (all weeks)

* Private GitHub repository, lecturer **jakorten** added as collaborator.
* A `README.md` at the root that says what works, on which board / Pi, and how to build it.
* Small, meaningful commits. "final version 3 really final" is not a commit message.
* Tag the commit you hand in with the tag named in the hand-out.
