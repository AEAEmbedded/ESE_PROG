# ADR-001: Use CAN bus for sensor communication

## Status
Accepted

## Context
We need communication between the compression sensor node 
and the main controller. Distance is ~40cm, environment 
includes stepper motors for lung simulation.

## Options Considered
1. I2C - simple, but limited distance, noise sensitive
2. SPI - fast, but requires many wires, limited distance
3. CAN - robust, standard in medical, designed for noise
4. RS-485 - robust, but requires protocol design

## Decision
CAN bus using MCP2515 controller.

## Consequences
- Additional component cost (~€3 per node)
- Team needs CAN protocol knowledge
- Easy to add more sensor nodes later
- Compliant with medical device expectations

## References
- MCP2515 Datasheet
- ISO 11898 (CAN standard)