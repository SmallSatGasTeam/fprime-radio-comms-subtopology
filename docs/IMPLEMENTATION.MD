# Implementation Strategy: Structural Scaffolding

This project ships the F' structural scaffolding for a radio communication stack — the
component interfaces, the subtopology wiring, and an example deployment. The
radio-specific behavior (framing algorithm, CRC, beacon logic, I2C configuration) is
intentionally left as stubs for the integrating team to implement against their hardware's datasheet.

## What is provided

- **Component definitions (.fpp)**: ports, commands, events, and telemetry for the
  link-protocol and management components.
- **Subtopology connections (.fpp)**: how the framing components connect to each other and
  to standard F' services (`ComQueue`, `FrameAccumulator`, `FprimeRouter`, `BufferManager`,
  `ComStub`).
- **Deployment example**: a full F' deployment showing the wiring.

## What is stubbed

The behavior-bearing bodies are intentional no-ops / pass-throughs:

- Framing / deframing / frame detection (`RadioFramer`, `RadioDeframer`,
  `RadioFrameDetector`) — preamble, sync word, length field, and CRC handling.
- Beacon management and downlink routing (`TransceiverCommsManager`).
- I2C transceiver configuration (`TransceiverConfigurationManager`).

Operators implement these handlers against their radio's documentation. The project ships no radio-specific values (polynomials, sync words, callsigns, vendor command sets) and no unit tests.
