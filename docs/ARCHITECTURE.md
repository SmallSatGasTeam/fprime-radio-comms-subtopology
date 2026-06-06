# Architecture Specification: Radio Communication Stack for F'

## System Overview

This repository provides an F' (F Prime) structural scaffolding for a generic radio communication stack. It shows how to wire custom framing and deframing components into an F' flight software system alongside standard services like `ComQueue`, `FrameAccumulator`, `FprimeRouter`, `BufferManager`, and `ComStub`.

The radio-specific behavior is intentionally stubbed. This project is a structural template and integration reference; operators supply the actual framing algorithm, beacon logic, and I2C configuration by implementing the marked stub bodies against their hardware's datasheet.

## Directory Architecture

```
fprime-radio-comms-subtopology/
├── Components/
│   ├── TransceiverCommsManager/         # Downlink/beacon gateway interface (stubbed)
│   └── TransceiverConfigurationManager/ # I2C transceiver configuration interface (stubbed)
│
├── LinkProtocols/
│   └── RadioLinkProtocol/          # The F' framing/deframing components
│       ├── RadioFramer/            # Wraps packets in radio/link frames (stubbed)
│       ├── RadioDeframer/          # Strips headers from incoming frames (stubbed)
│       ├── RadioFrameDetector/     # Finds frame boundaries in a byte stream (stubbed)
│       └── RadioProtocol/          # FPP subtopology connecting the stack
│
└── Deployments/
    └── RadioDeployment/            # Example deployment using the comms stack
```

## Subsystem Breakdown

### `LinkProtocols/RadioLinkProtocol`

The framing stack.

- **RadioFramer**: F' component on the downlink path. Receives `Fw::Buffer` packets, allocates a buffer for the framed data, and forwards it toward the communication driver. The framing body is a stubbed pass-through.
- **RadioDeframer**: F' component on the uplink path. Receives accumulated frames and routes them to the `FprimeRouter`. The deframing body is a stubbed pass-through.
- **RadioFrameDetector**: An implementation of the `Svc::FrameDetector` interface, used by `Svc::FrameAccumulator` to find frames in a continuous byte stream. The detection body is a stub that treats any available data as a single frame.
- **RadioProtocol**: An FPP subtopology that encapsulates the instances and connections of the framing stack, making it easy to drop into any F' deployment.

### `Components/`

Higher-level management component interfaces. Each defines its F' ports, commands, events, and telemetry, but ships with stubbed behavior (see each component's `docs/sdd.md`).

- **TransceiverCommsManager**: Intended downlink gateway and beacon controller — all outgoing data would route through it, and it would manage periodic beacon transmission.
- **TransceiverConfigurationManager**: Intended I2C configuration of radio parameters such as frequency, callsigns, and power mode.

## Dependencies

- **F' (F Prime)**: The underlying flight software framework. Greater than v4.
- **C++11/17**: For component implementations.
- **FPP**: For structural modeling and connection definitions.
