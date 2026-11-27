#!/usr/bin/env python3
"""
monitor.py - Print every message on the scenario bus.

HAN University of Applied Sciences - Programming 6 - Johan Korten - v1.0 2026

A debugging aid: subscribes to scenario/# and prints topic and payload with a
timestamp. The command line tool `mosquitto_sub -t 'scenario/#' -v` does the
same without Python; this version shows how a subscriber-only node looks and
is the seed for an instructor view.

Usage:
    python3 monitor.py [--host HOST] [--filter 'scenario/#']
"""

from __future__ import annotations

import argparse
import datetime
import time

import topics
from mqtt_node import MqttNode, add_broker_arguments, configure_logging


class Monitor(MqttNode):
    def __init__(self, topic_filter: str, host: str, port: int) -> None:
        super().__init__(client_id="monitor", host=host, port=port)
        self.subscribe(topic_filter, self._print)

    @staticmethod
    def _print(topic: str, payload: bytes) -> None:
        stamp = datetime.datetime.now().strftime("%H:%M:%S")
        print(f"{stamp}  {topic:<48} {payload.decode(errors='replace')}", flush=True)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    add_broker_arguments(parser)
    parser.add_argument("--filter", default=topics.EVERYTHING, help="topic filter (default: scenario/#)")
    args = parser.parse_args()
    configure_logging(args.verbose)

    monitor = Monitor(args.filter, args.host, args.port)
    monitor.connect()
    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        pass
    finally:
        monitor.disconnect()


if __name__ == "__main__":
    main()
