"""
mqtt_node.py - Small base class around paho-mqtt for every component.

HAN University of Applied Sciences - Programming 6 - Johan Korten - v1.0 2026

Every component of the system (engine, SimDevice, instructor view, bridge) is
an MQTT node. This class hides the paho details so a component only talks in
terms of topics and payloads. Subclasses override `on_connected` to subscribe
and use `subscribe(filter, handler)` to route messages to methods.

Works with paho-mqtt 2.x (CallbackAPIVersion.VERSION2).
"""

from __future__ import annotations

import logging
from typing import Callable

import paho.mqtt.client as mqtt

log = logging.getLogger(__name__)

Handler = Callable[[str, bytes], None]


class MqttNode:
    """One MQTT connection with topic-filter based dispatch."""

    def __init__(self, client_id: str, host: str = "localhost", port: int = 1883) -> None:
        self._host = host
        self._port = port
        self._handlers: list[tuple[str, Handler]] = []
        self._client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2, client_id=client_id)
        self._client.on_connect = self._on_connect
        self._client.on_message = self._on_message

    # --- lifecycle --------------------------------------------------------

    def set_last_will(self, topic: str, payload: str) -> None:
        """Message the broker publishes when this node drops off unexpectedly.
        Call before connect()."""
        self._client.will_set(topic, payload, qos=1, retain=True)

    def connect(self) -> None:
        log.info("connecting to %s:%d", self._host, self._port)
        self._client.connect(self._host, self._port, keepalive=30)
        self._client.loop_start()  # network thread; callbacks run on it

    def disconnect(self) -> None:
        self._client.loop_stop()
        self._client.disconnect()

    # --- messaging --------------------------------------------------------

    def publish(self, topic: str, payload: str, retain: bool = False) -> None:
        log.debug("-> %s %s", topic, payload)
        self._client.publish(topic, payload, qos=1, retain=retain)

    def subscribe(self, topic_filter: str, handler: Handler) -> None:
        """Route every message matching `topic_filter` (may contain + and #)
        to `handler(topic, payload)`. May be called before or after connect()."""
        self._handlers.append((topic_filter, handler))
        if self._client.is_connected():
            self._client.subscribe(topic_filter, qos=1)

    # --- hooks for subclasses --------------------------------------------

    def on_connected(self) -> None:
        """Called after a successful connect. Subscribe and announce here."""

    # --- paho callbacks ---------------------------------------------------

    def _on_connect(self, client, userdata, flags, reason_code, properties) -> None:  # noqa: ANN001
        if reason_code != 0:
            log.error("connect failed: %s", reason_code)
            return
        log.info("connected")
        for topic_filter, _ in self._handlers:
            client.subscribe(topic_filter, qos=1)
        self.on_connected()

    def _on_message(self, client, userdata, message) -> None:  # noqa: ANN001
        for topic_filter, handler in self._handlers:
            if mqtt.topic_matches_sub(topic_filter, message.topic):
                try:
                    handler(message.topic, message.payload)
                except Exception:  # keep the network thread alive
                    log.exception("handler for %s failed on %s", topic_filter, message.topic)


def add_broker_arguments(parser) -> None:  # noqa: ANN001
    """Common --host/--port/--verbose options for all scripts."""
    parser.add_argument("--host", default="localhost", help="MQTT broker host (default: localhost)")
    parser.add_argument("--port", type=int, default=1883, help="MQTT broker port (default: 1883)")
    parser.add_argument("-v", "--verbose", action="store_true", help="show every published message")


def configure_logging(verbose: bool) -> None:
    logging.basicConfig(
        level=logging.DEBUG if verbose else logging.INFO,
        format="%(asctime)s %(levelname)-7s %(name)s: %(message)s",
        datefmt="%H:%M:%S",
    )
