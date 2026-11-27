#!/usr/bin/env python3
"""
virtual_thermometer.py - A virtual thermometer SimDevice.

HAN University of Applied Sciences - Programming 6 - Johan Korten - v1.0 2026

This is the "connection: virtual" variant from the challenge document: a small
program that publishes and subscribes the SimDevice topics directly. For the
engine it is indistinguishable from the hacked donor thermometer. Use it to
test your engine before the firmware works, and use its topics and payloads as
the contract for your firmware.

Behavior:
  * announces itself on scenario/devices/<ID>/status (retained) with a last
    will so the broker reports a crash as "disconnected",
  * remembers the last scenario/devices/<ID>/command/{body,surface} values,
  * Enter in the terminal = pull the trigger: publishes
    scenario/devices/<ID>/event/measured with the current value and mode.

Usage:
    python3 virtual_thermometer.py [--host HOST] [--id thermo-01]
    Enter      measure (pull the trigger)
    b / s      switch to body / surface mode
    q          quit
"""

from __future__ import annotations

import argparse
import logging
import threading
from typing import Optional

import topics
from mqtt_node import MqttNode, add_broker_arguments, configure_logging

log = logging.getLogger("thermometer")


class VirtualThermometer(MqttNode):
    MODES = ("body", "surface")

    def __init__(self, device_id: str, host: str, port: int) -> None:
        super().__init__(client_id=f"virtual-{device_id}", host=host, port=port)
        self._id = device_id
        self._lock = threading.Lock()
        self._mode = "body"
        self._values: dict[str, Optional[float]] = {m: None for m in self.MODES}
        self._battery = 100

        self.set_last_will(topics.device_status(device_id),
                           topics.encode(connected=False, reason="last will"))
        self.subscribe(topics.device_command_filter(device_id), self._on_command)

    # --- what the engine tells us ----------------------------------------

    def _on_command(self, topic: str, payload: bytes) -> None:
        _, _, param = topics.split_device_topic(topic)
        data = topics.decode(payload)
        if param not in self.MODES:
            log.warning("unknown command parameter %s", param)
            return
        with self._lock:
            self._values[param] = float(data["value"])
        log.info("command %s = %s %s", param, data["value"], data.get("unit", ""))

    # --- what the nurse does ---------------------------------------------

    def set_mode(self, mode: str) -> None:
        with self._lock:
            self._mode = mode
        log.info("mode -> %s", mode)

    def pull_trigger(self) -> None:
        with self._lock:
            mode, value = self._mode, self._values[self._mode]
        if value is None:
            log.warning("no %s temperature received from the engine yet", mode)
            return
        log.info("measured %.1f C (%s)", value, mode)
        self.publish(topics.device_event(self._id, "measured"),
                     topics.encode(value=value, unit="C", mode=mode))

    # --- lifecycle --------------------------------------------------------

    def on_connected(self) -> None:
        self.publish(topics.device_status(self._id),
                     topics.encode(connected=True, battery=self._battery, type="thermometer"),
                     retain=True)

    def shutdown(self) -> None:
        self.publish(topics.device_status(self._id),
                     topics.encode(connected=False, reason="shutdown"), retain=True)
        self.disconnect()


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    add_broker_arguments(parser)
    parser.add_argument("--id", default="thermo-01", help="device ID used in the topics (default: thermo-01)")
    args = parser.parse_args()
    configure_logging(args.verbose)

    device = VirtualThermometer(args.id, args.host, args.port)
    device.connect()
    print("Enter = pull trigger, b/s = body/surface mode, q = quit")
    try:
        while True:
            key = input().strip().lower()
            if key == "q":
                break
            if key in ("b", "body"):
                device.set_mode("body")
            elif key in ("s", "surface"):
                device.set_mode("surface")
            else:
                device.pull_trigger()
    except (KeyboardInterrupt, EOFError):
        pass
    finally:
        device.shutdown()


if __name__ == "__main__":
    main()
