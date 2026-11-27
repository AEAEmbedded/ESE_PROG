"""
topics.py - The MQTT topic set and payload conventions of the challenge.

HAN University of Applied Sciences - Programming 6 - Johan Korten - v1.0 2026

This file is the single source of truth for topic names in the Python
skeleton. It mirrors Table "Minimal MQTT topic set" of the challenge document
and `cpp/mqtt/include/topics.hpp`. If you extend the topic set, extend it here
and keep the existing names and payloads intact, so components of different
teams keep working together.

All payloads are JSON objects with at least a "timestamp" (Unix epoch seconds).
"""

from __future__ import annotations

import json
import time
from typing import Any

# --- Fixed topics ---------------------------------------------------------

STATE_CURRENT = "scenario/state/current"            # engine -> everyone
STATE_VITALS = "scenario/state/vitals"              # engine -> everyone
INTERVENTIONS_PERFORMED = "scenario/interventions/performed"  # devices, NFC, instructor view -> engine
INTERVENTIONS_FEEDBACK = "scenario/interventions/feedback"    # engine -> instructor view
DEBRIEF = "scenario/debrief"                        # engine -> instructor view

# Wildcards for subscribers
CONTROL_ALL = "scenario/control/+"
DEVICES_ALL = "scenario/devices/#"
DEVICE_STATUS_ALL = "scenario/devices/+/status"
DEVICE_EVENTS_ALL = "scenario/devices/+/event/+"
EVERYTHING = "scenario/#"

# Known control commands and intervention sources
CONTROL_COMMANDS = ("load", "start", "pause", "stop")
SOURCE_DETECTED = "detected"
SOURCE_SIMULATED = "simulated"


# --- Topic builders -------------------------------------------------------

def control(command: str) -> str:
    """scenario/control/<command>, command in CONTROL_COMMANDS."""
    return f"scenario/control/{command}"


def device_command(device_id: str, param: str) -> str:
    """scenario/devices/<ID>/command/<param>, e.g. the body temperature to show."""
    return f"scenario/devices/{device_id}/command/{param}"


def device_command_filter(device_id: str) -> str:
    """All commands for one device."""
    return f"scenario/devices/{device_id}/command/+"


def device_event(device_id: str, event: str) -> str:
    """scenario/devices/<ID>/event/<event>, e.g. 'measured', 'shock'."""
    return f"scenario/devices/{device_id}/event/{event}"


def device_status(device_id: str) -> str:
    """scenario/devices/<ID>/status (published retained, with a last will)."""
    return f"scenario/devices/{device_id}/status"


def split_device_topic(topic: str) -> tuple[str, str, str]:
    """
    Split 'scenario/devices/<ID>/<kind>/<name>' into (device_id, kind, name).
    For a status topic, kind is 'status' and name is ''.
    """
    parts = topic.split("/")
    if len(parts) < 4 or parts[0] != "scenario" or parts[1] != "devices":
        raise ValueError(f"not a device topic: {topic}")
    device_id, kind = parts[2], parts[3]
    name = parts[4] if len(parts) > 4 else ""
    return device_id, kind, name


# --- Payload helpers ------------------------------------------------------

def now() -> int:
    """Unix epoch seconds, the timestamp format used in all payloads."""
    return int(time.time())


def encode(**fields: Any) -> str:
    """Build a JSON payload; a timestamp is added when not given."""
    fields.setdefault("timestamp", now())
    return json.dumps(fields, separators=(",", ":"))


def decode(payload: bytes | str) -> dict[str, Any]:
    """Parse a JSON payload. Raises ValueError on malformed input."""
    data = json.loads(payload)
    if not isinstance(data, dict):
        raise ValueError("payload is not a JSON object")
    return data
