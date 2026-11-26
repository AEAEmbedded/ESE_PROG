# Zephyr Workshop - Your First RTOS

**HAN University of Applied Sciences**
**Software Engineering - Programming 6**
**Johan Korten - v1.0 2026**

---

## What This Workshop Is

You know the Arduino way: one big `loop()` that does everything. This workshop
shows why that stops working when timing matters, and introduces the solution:
a **Real-Time Operating System (RTOS)**. We use **Zephyr**, the Linux
Foundation's open-source RTOS used by Nordic, ST, NXP and many medical-device
companies.

No prior RTOS knowledge needed. No hardware needed either: everything runs on
**QEMU**, an emulated ARM Cortex-M3 board (`qemu_cortex_m3`) on your own laptop.

**Link to the CMake workshop:** Zephyr builds with CMake + Ninja under the
hood, driven by the `west` tool. Everything you learned there transfers
directly: `west build` is `cmake -S . -B build && cmake --build build` with
superpowers.

---

## ⚠️ Prerequisites - Install BEFORE the Workshop

The setup downloads about 2 GB. **Do this at home**, not on the campus Wi-Fi
during the workshop.

> **Golden rule: use a path WITHOUT spaces.** Do not put the Zephyr workspace
> or your apps in OneDrive, iCloud or a folder like `My Documents`. Use
> `~/zephyrproject`. Paths with spaces break embedded build tools in creative
> ways.

### 1. Install the base tools

**macOS:**
```bash
brew install cmake ninja python3 qemu dtc
```

**Linux (Ubuntu/Debian):**
```bash
sudo apt install cmake ninja-build python3-venv qemu-system-arm device-tree-compiler
```

**Windows:** use **WSL2** (Ubuntu) and follow the Linux steps inside it. This
is by far the smoothest route; see
<https://learn.microsoft.com/windows/wsl/install>.

### 2. Create the Zephyr workspace

```bash
mkdir ~/zephyrproject && cd ~/zephyrproject
python3 -m venv .venv
source .venv/bin/activate          # do this in every new terminal!
pip install west
west init --mr v4.2.0 .
west update                        # big download: get coffee
pip install -r zephyr/scripts/requirements-base.txt
west sdk install -t arm-zephyr-eabi
```

> **Watch out with `west init`:** the `.` means "this directory", so make
> sure you are really inside `~/zephyrproject` (check with `pwd`). If you
> run it from your home directory, your entire home folder becomes a west
> workspace and you get a stray `~/zephyr` + hidden `~/.west`. Fix:
> `rm -rf ~/.west ~/zephyr` and redo this step in the right place.

### 3. Verify it works

```bash
cd ~/zephyrproject
cp -r <this repo>/Workshops/Zephyr/Source/01_hello apps/01_hello
cd apps/01_hello
west build -b qemu_cortex_m3
west build -t run                  # quit QEMU with Ctrl-A, then X
```

If you see `Hello from Zephyr on board: qemu_cortex_m3`, you are ready.

---

## Workshop Structure

```
Source/
├── 01_hello/          # Step 1: toolflow - build and run on QEMU
├── 02_superloop/      # Step 2: the PROBLEM - one slow job freezes everything
├── 03_threads/        # Step 3: the SOLUTION - threads and priorities
├── 04_sensor_queue/   # Step 4: threads communicating via a message queue
├── 05_devicetree/     # Step 5: hardware and config described in data
└── 06_blinky/         # Step 6 (bonus): the same code on REAL hardware
```

Copy the apps into `~/zephyrproject/apps/` (no spaces!) and build each one the
same way:

```bash
cd ~/zephyrproject && source .venv/bin/activate
cd apps/<app>
west build -b qemu_cortex_m3
west build -t run        # Ctrl-A, X to quit
```

If a build behaves strangely after switching apps or boards:
`west build -p` forces a pristine (clean) build - the Zephyr equivalent of
`rm -rf build`.

---

## Step 1: 01_hello

**Goal:** see the toolflow work, and recognize the CMake underneath.

A Zephyr app is three files: `CMakeLists.txt` (4 lines - compare that with the
CMake workshop!), `prj.conf` (empty for now) and `src/main.c`. `west build`
configures with CMake, builds with Ninja, links the whole RTOS plus your app
into one firmware image, and QEMU boots it.

---

## Step 2: 02_superloop

**Goal:** experience the problem an RTOS solves.

One loop, two jobs: a heartbeat that must tick every 100 ms, and heavy work
every 3 s that takes a full second. Watch the output: every time the heavy
work runs, **the heartbeat stops for a second**. In a medical device, that
second matters. This is the Arduino `loop()` pattern hitting its limit.

---

## Step 3: 03_threads

**Goal:** the same two jobs, now as RTOS threads - and the problem is gone.

Each job becomes a **thread** with a **priority** (lower number = more
important in Zephyr). The **scheduler** interrupts (preempts) the slow thread
whenever the fast one needs the CPU. The heartbeat now ticks right through the
heavy work.

**Experiments (do them!):**
1. Swap the priorities. Now the heartbeat starves during heavy work. Being
   explicit about what is important is a design decision, not an accident.
2. Set the heartbeat to 10 ms. Still solid.

---

## Step 4: 04_sensor_queue

**Goal:** threads that cooperate without shared variables.

A sensor thread simulates a body thermometer (the patient slowly develops a
fever); a monitor thread receives each reading through a **message queue**
(`k_msgq`) and raises an alarm above 38 °C. No shared globals, no race
conditions: all data travels through the queue. This producer/consumer pattern
is exactly how the parts of a larger embedded system - like your final
challenge - stay decoupled.

---

## Step 5: 05_devicetree

**Goal:** hardware and configuration live in data, not in code.

Two mechanisms you must be able to tell apart:

- **Devicetree** answers *"what hardware is there and where?"* Every board
  ships a `.dts` description; your app extends it with an `app.overlay`. At
  build time this becomes C macros: the app reads the console UART's address
  and our own `fever-threshold-mdeg` setting from the devicetree instead of
  hard-coding them.
- **Kconfig** (`prj.conf`) answers *"which software features are compiled
  in?"* - logging on/off, stack sizes, subsystems. Explore it live with
  `west build -t menuconfig`.

After building, open `build/zephyr/zephyr.dts`: the merged result of board +
overlay. **That generated file is the truth** - check it before trusting any
devicetree advice, including from an AI.

Changing the fever threshold means editing the overlay and rebuilding - no C
code touched. Recognize the Open-Closed principle from the final challenge's
configuration tables.

---

## Step 6 (bonus): 06_blinky - Real Hardware

**Goal:** the devicetree promise, delivered. Each group has a **Seeed XIAO
nRF52840** and a **Raspberry Pi Pico 2 W**: one application, zero code
changes, two completely different boards.

`main.c` never names a pin. It asks for `DT_ALIAS(led0)` and the board's
devicetree decides what that means.

Good news: **both boards flash with zero extra tools.** They each have a UF2
bootloader: the board shows up as a USB drive and you drag
`build/zephyr/zephyr.uf2` onto it.

### Seeed XIAO nRF52840

```bash
cd ~/zephyrproject/apps/06_blinky
west build -p -b xiao_ble/nrf52840
```

**Double-tap** the tiny reset button (next to the USB-C connector) - the
board appears as a USB drive. Copy `build/zephyr/zephyr.uf2` onto it; the
board reboots into your firmware and the **red** LED blinks.

Why red? `led0` is the red channel of the onboard RGB LED. Open the merged
`build/zephyr/zephyr.dts` and find `led1` (green) and `led2` (blue).
**Mini-exercise:** make it blink green by changing one word in `main.c`.

### Raspberry Pi Pico 2 W

```bash
west build -p -b rpi_pico2/rp2350a/m33
```

Hold the **BOOTSEL** button while plugging in the USB cable, then copy
`build/zephyr/zephyr.uf2` onto the drive that appears.

**The catch (and the lesson):** on the Pico 2 **W**, the onboard LED is wired
to the WiFi chip, not to a normal GPIO - and the stock board devicetree does
not even define a `led0` alias. Our
`boards/rpi_pico2_rp2350a_m33.overlay` therefore maps `led0` to an
**external LED on pin GP15** (LED + ~220 ohm resistor to GND, breadboard).
Zephyr picks the overlay up automatically by its file name. Open it: this is
devicetree doing its actual job - absorbing a board difference without
touching a line of C.

**Experiment:** move the LED to another pin and make it work by editing only
the overlay. Verify your change in `build/zephyr/zephyr.dts` before flashing.

---

## Opening the Projects in VS Code

Open **one app folder at a time** (not the whole workspace):

```bash
code ~/zephyrproject/apps/01_hello
```

Red squiggles everywhere? Normal: the editor does not know Zephyr's include
paths until you tell it. Two steps:

1. **Build first** - `west build -b qemu_cortex_m3` generates
   `build/compile_commands.json`, the exact compiler commands for every file.
2. **Point the editor at it.** With the Microsoft **C/C++** extension, create
   `.vscode/settings.json` in the app folder:

   ```json
   {
       "C_Cpp.default.compileCommands": "${workspaceFolder}/build/compile_commands.json"
   }
   ```

   (Using the **clangd** extension instead? Add
   `"clangd.arguments": ["--compile-commands-dir=${workspaceFolder}/build"]`.)

Reload the window and IntelliSense understands `<zephyr/kernel.h>`, devicetree
macros included. After switching boards, rebuild so the file is regenerated.

---

## Using AI Assistants with Zephyr (read this!)

LLMs (Claude, ChatGPT, Copilot) are genuinely useful for Zephyr's tooling, and
genuinely dangerous if you paste blindly. The rules:

1. **Always state your Zephyr version and board** ("Zephyr v4.2.0,
   qemu_cortex_m3"). APIs, Kconfig symbols and devicetree bindings change
   between versions; an answer for v2.x will mislead you in v4.x.
2. **Paste the complete error**, not a summary. Devicetree errors are cryptic
   (`__device_dts_ord_17 undeclared` means: that devicetree node exists but
   has no enabled driver) - the full text is exactly what an LLM is good at
   decoding.
3. **Verify every `CONFIG_` symbol before using it.** LLMs invent
   plausible-looking Kconfig symbols. Check with `west build -t menuconfig`
   (press `/` to search) - if it is not there, it does not exist.
4. **Verify devicetree claims against `build/zephyr/zephyr.dts`.** That file
   is generated from your actual build; the model's memory is not.
5. **Ask "why", not just "fix it."** In the final interview you must explain
   your own code. "Explain why this overlay fixes the error" teaches you
   something; a pasted fix does not.

Good first prompt when you are stuck:

> I am using Zephyr v4.2.0 on qemu_cortex_m3 with west. Here are my prj.conf,
> app.overlay and the complete build error: [paste]. Explain what is wrong and
> why, before proposing a fix.

---

## Troubleshooting

| Symptom | Cause / fix |
|---|---|
| `west: command not found` | Activate the venv: `source ~/zephyrproject/.venv/bin/activate` |
| `ZEPHYR_BASE not set` / CMake cannot find Zephyr | Run `west build` from inside the workspace (`~/zephyrproject/...`), not from a random directory |
| SDK/toolchain not found | `west sdk install -t arm-zephyr-eabi` (inside the workspace, venv active) |
| Build errors after switching apps/boards | Pristine build: `west build -p` |
| Stuck inside QEMU | Quit with **Ctrl-A, then X** |
| Weird path errors | Your workspace or app path contains spaces - move it to `~/zephyrproject` |
| VS Code "Zephyr IDE"/"Workbench" extension complains (`ZEPHYR_BASE not set`, "No setup state available") | Do NOT run its setup wizard (it installs a duplicate Zephyr). Either disable the extension (this workshop only needs plain `west` in the terminal), or point it at the existing install: add `export ZEPHYR_BASE=$HOME/zephyrproject/zephyr` and `export ZEPHYR_SDK_INSTALL_DIR=$HOME/zephyr-sdk-0.17.2` to your shell profile, start VS Code from a fresh terminal, choose "Continue" in the popup |

---

## Further Learning

- [Zephyr Getting Started](https://docs.zephyrproject.org/latest/develop/getting_started/index.html)
- [Zephyr Kernel Services (threads, queues, timers)](https://docs.zephyrproject.org/latest/kernel/services/index.html)
- [Introduction to Devicetree](https://docs.zephyrproject.org/latest/build/dts/intro.html)
- [Nordic DevAcademy - free Zephyr course](https://academy.nordicsemi.com/)

---

*This workshop is part of the Software Engineering curriculum at HAN
University of Applied Sciences. It builds on the CMake workshop: west drives
CMake + Ninja, exactly as previewed there.*
