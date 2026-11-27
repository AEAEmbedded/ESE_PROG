#!/usr/bin/env python3
"""
engine_stub.py - A stand-in for the scenario engine on the MQTT bus.

HAN University of Applied Sciences - Programming 6 - Johan Korten - v1.0 2026

What it does:
  * loads a scenario JSON and sits in its initial state,
  * publishes scenario/state/current and scenario/state/vitals once per second,
  * reacts to scenario/control/{load,start,pause,stop},
  * logs interventions and device events, and answers an intervention with a
    feedback message so the round trip is visible,
  * sends the current body temperature to every SimDevice that announces
    itself on scenario/devices/<ID>/status.

What it deliberately does NOT do: run the state machine (time-based and
intervention-triggered transitions), interpolate, score Dutch PEWS or build a
debrief. Those are the Must and Should features of your project. Use this stub
to get your SimDevice and instructor view talking before your own engine is
ready, then replace it.

Usage:
    python3 engine_stub.py [--host HOST] [--scenario ../scenarios/anaphylaxis.json]
"""

from __future__ import annotations

import argparse
import json
import logging
import pathlib
import threading
import time
from typing import Any

import topics
from mqtt_node import MqttNode, add_broker_arguments, configure_logging

log = logging.getLogger("engine")

DEFAULT_SCENARIO = pathlib.Path(__file__).resolve().parent.parent / "scenarios" / "anaphylaxis.json"


class Scenario:
    """Thin wrapper around the scenario JSON. Your real engine will replace
    this by proper State/Transition/Intervention classes."""

    def __init__(self, data: dict[str, Any]) -> None:
        self._data = data
        self.initial_state_id = next(
            (sid for sid, s in data["states"].items() if s.get("type") == "initial"),
            next(iter(data["states"])),
        )

    @classmethod
    def from_file(cls, path: pathlib.Path) -> "Scenario":
        with path.open(encoding="utf-8") as f:
            return cls(json.load(f))

    @property
    def title(self) -> str:
        return self._data.get("metadata", {}).get("title", "untitled")

    def vitals(self, state_id: str) -> dict[str, Any]:
        return dict(self._data["states"][state_id].get("vitals", {}))


class EngineStub(MqttNode):
    def __init__(self, scenario: Scenario, host: str, port: int) -> None:
        super().__init__(client_id="engine-stub", host=host, port=port)
        self._lock = threading.Lock()
        self._scenario = scenario
        self._state_id = scenario.initial_state_id
        self._entered_at = time.monotonic()
        self._running = False

        self.subscribe(topics.CONTROL_ALL, self._on_control)
        self.subscribe(topics.INTERVENTIONS_PERFORMED, self._on_intervention)
        self.subscribe(topics.DEVICE_EVENTS_ALL, self._on_device_event)
        self.subscribe(topics.DEVICE_STATUS_ALL, self._on_device_status)

    # --- publishing -------------------------------------------------------

    def tick(self) -> None:
        """Publish the current state and vitals. Called once per second."""
        with self._lock:
            if not self._running:
                return
            state_id = self._state_id
            time_in_state = round(time.monotonic() - self._entered_at, 1)
            vitals = self._scenario.vitals(state_id)

        self.publish(topics.STATE_CURRENT, topics.encode(
            state_id=state_id, state_name=state_id.replace("_", " ").title(),
            time_in_state=time_in_state))
        # TODO (your engine): compute the Dutch PEWS for this age band from a
        # configuration table and fill in ews / ews_color.
        self.publish(topics.STATE_VITALS, topics.encode(
            state_id=state_id, time_in_state=time_in_state, vitals=vitals,
            ews=None, ews_color=None))

    def _send_temperature(self, device_id: str) -> None:
        temp = self._scenario.vitals(self._state_id).get("Temp")
        if temp is None:
            return
        self.publish(topics.device_command(device_id, "body"),
                     topics.encode(value=temp, unit="C"))

    # --- handlers (run on the network thread) -----------------------------

    def _on_control(self, topic: str, payload: bytes) -> None:
        command = topic.rsplit("/", 1)[-1]
        log.info("control: %s", command)
        with self._lock:
            if command == "load":
                self._scenario = Scenario(topics.decode(payload))
                self._state_id = self._scenario.initial_state_id
                self._entered_at = time.monotonic()
                self._running = False
                log.info("loaded scenario '%s'", self._scenario.title)
            elif command == "start":
                self._entered_at = time.monotonic()
                self._running = True
            elif command == "pause":
                self._running = False
            elif command == "stop":
                self._running = False
                self._state_id = self._scenario.initial_state_id
            else:
                log.warning("unknown control command %s", command)

    def _on_intervention(self, topic: str, payload: bytes) -> None:
        data = topics.decode(payload)
        log.info("intervention: %s (%s)", data.get("intervention"), data.get("source"))
        # TODO (your engine): look up the transition for this intervention,
        # compare against optimal_within_s and switch state (with a ramp).
        self.publish(topics.INTERVENTIONS_FEEDBACK, topics.encode(
            intervention=data.get("intervention"), result="not_evaluated",
            note="engine stub: transitions are not implemented"))

    def _on_device_event(self, topic: str, payload: bytes) -> None:
        device_id, _, event = topics.split_device_topic(topic)
        log.info("device %s event %s: %s", device_id, event, payload.decode(errors="replace"))

    def _on_device_status(self, topic: str, payload: bytes) -> None:
        device_id, _, _ = topics.split_device_topic(topic)
        data = topics.decode(payload)
        log.info("device %s status: %s", device_id, data)
        if data.get("connected"):
            self._send_temperature(device_id)

    def on_connected(self) -> None:
        with self._lock:
            running = self._running
        log.info("scenario '%s', initial state '%s'. %s", self._scenario.title, self._state_id,
                 "Publishing." if running else "Waiting for scenario/control/start (or pass --autostart).")

    def start(self) -> None:
        with self._lock:
            self._running = True
            self._entered_at = time.monotonic()


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    add_broker_arguments(parser)
    parser.add_argument("--scenario", type=pathlib.Path, default=DEFAULT_SCENARIO)
    parser.add_argument("--autostart", action="store_true", help="start publishing without waiting for scenario/control/start")
    args = parser.parse_args()
    configure_logging(args.verbose)

    engine = EngineStub(Scenario.from_file(args.scenario), args.host, args.port)
    engine.connect()
    if args.autostart:
        engine.start()
    try:
        while True:
            engine.tick()
            time.sleep(1.0)
    except KeyboardInterrupt:
        pass
    finally:
        engine.disconnect()


if __name__ == "__main__":
    main()
