# MqttSkeleton - Starter Code for the MQTT Bus of the Challenge

**HAN University of Applied Sciences**
**Software Engineering - Programming 6 - ESE Health Programming Challenge 2026-2027**
**Johan Korten - v1.0 2026**

---

## What This Is

The challenge document says: *every component is a separate MQTT node, so a
team can replace one component without touching the others.* This skeleton is
the smallest set of nodes that shows that promise working:

| Component | Python | C++ |
|---|---|---|
| Topic set and payload conventions | `python/topics.py` | `cpp/mqtt/include/topics.hpp`, `payload.hpp` |
| Connection behind an interface | `python/mqtt_node.py` (`MqttNode`) | `cpp/mqtt/include/message_bus.hpp` (`IMessageBus`) + `mosquitto_bus.hpp` |
| Engine stub (publishes state and vitals) | `python/engine_stub.py` | - |
| Virtual thermometer SimDevice | `python/virtual_thermometer.py` | `cpp/devices/` + `cpp/app/` |
| Bus monitor | `python/monitor.py` | `mosquitto_sub -t 'scenario/#' -v` |
| Unit tests without a broker | - | `cpp/tests/` (CppUTest, `FakeBus`) |

Python and C++ interoperate: the C++ thermometer works with the Python engine
stub and vice versa, because they share the topics and payloads below. Pick
the language you want for each component; the document allows any language
that runs on the Pi and talks MQTT.

Everything here is **starter code, not a solution**. The engine stub does not
run the state machine, does not interpolate and does not score Dutch PEWS.
Those are your Must and Should features. What the skeleton gives you is a
working bus on day one, so your SimDevice and instructor view can be developed
and tested before your own engine is ready.

```
MqttSkeleton/
├── README.md
├── scenarios/anaphylaxis.json   # the example scenario from the document, plus Temp/DBP/AVPU
├── python/
│   ├── requirements.txt         # paho-mqtt >= 2.0
│   ├── topics.py                # topic constants, builders, payload encode/decode
│   ├── mqtt_node.py             # MqttNode: paho connection + filter-based dispatch
│   ├── engine_stub.py           # publishes state/vitals, logs interventions and events
│   ├── virtual_thermometer.py   # Enter = pull trigger -> event/measured
│   └── monitor.py               # prints everything on scenario/#
└── cpp/                         # same layout as the ChallengeSkeleton of the CMake workshop
    ├── CMakeLists.txt, CMakePresets.json
    ├── mqtt/                    # sim_bus (topics, payload, IMessageBus) + sim_bus_mosquitto
    ├── devices/                 # VirtualThermometer, depends on IMessageBus only
    ├── app/                     # virtual_thermometer executable (wires MosquittoBus)
    └── tests/                   # CppUTest: topics, payload, thermometer with FakeBus
```

---

## 1. The Broker

Mosquitto runs on the Raspberry Pi. In hotspot mode the Pi is the network, so
every node connects to the Pi's address.

```bash
# Raspberry Pi (Raspberry Pi OS / Debian)
sudo apt install mosquitto mosquitto-clients libmosquitto-dev
# Allow connections from other machines (default since Mosquitto 2.0 is localhost only):
sudo tee /etc/mosquitto/conf.d/challenge.conf <<'CONF'
listener 1883
allow_anonymous true
CONF
sudo systemctl restart mosquitto

# Laptop, for development: macOS
brew install mosquitto && brew services start mosquitto
# Laptop: Windows -> https://mosquitto.org/download/ ; Linux -> same as the Pi
```

`allow_anonymous true` is fine for a training room on a private hotspot. Note
it as a known limitation in your design, not as a feature.

Check that the broker works, with two terminals:

```bash
mosquitto_sub -h <pi-address> -t 'scenario/#' -v
mosquitto_pub -h <pi-address> -t scenario/control/start -m '{"timestamp":0}'
```

---

## 2. Python Quick Start

```bash
cd python
python3 -m venv .venv && source .venv/bin/activate      # Windows: .venv\Scripts\activate
pip install -r requirements.txt

# Terminal 1: see everything on the bus
python3 monitor.py --host <broker>

# Terminal 2: the engine stand-in, publishing the 'onset' state once per second
python3 engine_stub.py --host <broker> --autostart

# Terminal 3: a virtual thermometer. Press Enter to pull the trigger.
python3 virtual_thermometer.py --host <broker> --id thermo-01

# Terminal 4: an instructor view that simulates an intervention
mosquitto_pub -h <broker> -t scenario/interventions/performed \
  -m '{"timestamp":0,"intervention":"epinephrine_im","source":"simulated"}'
```

`--host` defaults to `localhost`. Without `--autostart` the engine stub waits
for `scenario/control/start`.

---

## 3. C++ Quick Start

Needs CMake 3.20+, a C++17 compiler and libmosquitto (`libmosquitto-dev` on
the Pi, `brew install mosquitto` on macOS). The tests download CppUTest with
`FetchContent`, exactly like the ChallengeSkeleton.

```bash
cd cpp
cmake --preset debug             # or: cmake -S . -B build
cmake --build --preset debug
ctest --preset debug             # 16 unit tests, no broker needed
./build/debug/app/virtual_thermometer <broker> thermo-02
```

The C++ thermometer and the Python engine stub work together out of the box.

---

## 4. Topics and Payloads

The topic set is the one from the challenge document. All payloads are JSON
objects with at least a `timestamp` in Unix epoch seconds. Keep these names
and fields intact; add your own fields and topics next to them.

| Topic | Publisher | Payload (fields in addition to `timestamp`) |
|---|---|---|
| `scenario/control/{load,start,pause,stop}` | scenario editor | `load`: the scenario JSON; others: `{}` |
| `scenario/state/current` | engine | `state_id`, `state_name`, `time_in_state` (s) |
| `scenario/state/vitals` | engine | `state_id`, `time_in_state`, `vitals` {HR, SBP, DBP, RR, SpO2, Temp, AVPU}, `ews`, `ews_color` |
| `scenario/interventions/performed` | SimDevice, NFC reader, instructor view | `intervention` (id from the scenario), `source` (`detected` / `simulated`), optional `device_id` |
| `scenario/interventions/feedback` | engine | `intervention`, `result` (correct / wrong / late), timing |
| `scenario/devices/<ID>/command/<param>` | engine | `value`, `unit` (thermometer: param `body` or `surface`) |
| `scenario/devices/<ID>/event/<event>` | SimDevice | event-specific, e.g. `measured`: `value`, `unit`, `mode` |
| `scenario/devices/<ID>/status` | SimDevice (retained, with last will) | `connected` (bool), `battery`, `type`; on disconnect `reason` |
| `scenario/debrief` | engine | timeline at scenario end |

Two MQTT details that matter in practice, both used in the skeleton:

- **Retained status.** A SimDevice publishes its status with the retain flag,
  so a node that connects later (the engine, an instructor view) immediately
  learns which devices exist.
- **Last will.** A SimDevice registers a `connected: false` message as its
  last will before connecting. If the firmware crashes or the Wi-Fi drops,
  the broker publishes it. Without this, the instructor never sees that a
  device is gone.

The engine stub shows the consequence: when a device announces itself as
connected, the engine sends it the current temperature. Devices do not need
to be configured in the engine; they are discovered.

---

## 5. Using This in Your Project

1. Copy the parts you need into your team repository. The `topics` file is
   the contract between your components; keep one copy per language and keep
   them in sync.
2. Replace `engine_stub.py` by your own engine. Start with the state machine
   and `scenario/state/vitals`; the virtual thermometer and the monitor will
   show it working immediately.
3. Use the virtual thermometer's topics as the specification for your
   firmware. A `Thermometer` class that combines a display, a buzzer, a
   trigger and a `Connection` interface is the same shape as
   `VirtualThermometer` plus `IMessageBus`.
4. Write a virtual AED the same way: subscribe to `command/rhythm`, publish
   `event/pads_placed`, `event/analysis`, `event/shock`.
5. Test without a broker. `cpp/tests/fake_bus.hpp` is a test double for the
   bus; the Unit Testing workshop shows how to grow this into mocks.
6. For the scenario files and anything beyond flat payloads, use a real JSON
   library (`nlohmann/json` via `FetchContent` in C++, `json` in Python).
   `payload.hpp` is deliberately minimal.

---

## References

- Challenge document: `../ESE_Health_Finale_Challenge_2026-2027.pdf`
- ChallengeSkeleton (CMake layout, CppUTest): `../../Workshops/CMake/Source/ChallengeSkeleton/`
- Mosquitto: <https://mosquitto.org/documentation/>
- paho-mqtt (Python): <https://eclipse.dev/paho/files/paho.mqtt.python/html/>
- libmosquitto API: <https://mosquitto.org/api/files/mosquitto-h.html>
- Dutch PEWS: <https://dutchpews.com/wat-is-dutch-pews/>
