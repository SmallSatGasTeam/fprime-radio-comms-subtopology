# Architecture Specification: Radio Communication Stack for F'

## System Overview

This repository provides an F' (F Prime) structural scaffolding for a generic radio communication stack. It shows how to wire custom framing and deframing components into an F' flight software system alongside standard services like `ComQueue`, `FprimeRouter`, and `BufferManager`.

The framing logic itself is intentionally a stub. This project is a structural template and integration reference; operators supply the actual algorithm via `keys_template/TransceiverConfig.hpp` and the marked stub bodies.

## Directory Architecture

```
Open_Source_FPrime_Radio_Communications/
├── Components/
│   ├── TransceiverCommsManager/        # Manages beacon state and downlink routing
│   └── TransceiverConfigurationManager/ # Manages transceiver configuration via I2C
│
├── LinkProtocols/
│   └── RadioLinkProtocol/          # The F' framing/deframing implementation
│       ├── RadioFramer/            # Wraps packets in radio/link headers (Stubbed)
│       ├── RadioDeframer/          # Strips headers from incoming frames (Stubbed)
│       ├── RadioFrameDetector/     # Identifies frame boundaries in stream (Stubbed)
│       └── RadioProtocol/          # FPP Subtopology connecting the stack
│
├── Deployments/
│   └── RadioDeployment/       # Example deployment using the comms stack
│
└── keys_template/                      # IP Abstraction Layer
    ├── README_IP_PROTECTION.md         # Instructions for filling in radio parameters
    └── TransceiverConfig.hpp.example   # Template for radio parameters and keys
```

## Subsystem Breakdown

### `LinkProtocols/RadioLinkProtocol`
This is the heart of the communication stack implementation in F'.

- **RadioFramer**: An F' component that receives `Fw::Buffer` packets, allocates a new buffer for the framed data, and forwards it to the communication driver.
- **RadioDeframer**: An F' component that receives raw buffers, identifies the packet type, and routes it to the `FprimeRouter`.
- **RadioFrameDetector**: An implementation of the `Svc::FrameDetector` interface, used by the `Svc::FrameAccumulator` to find frames in a continuous byte stream.
- **Subtopology**: An FPP subtopology that encapsulates the instances and connections of the framing stack, making it easy to drop into any F' deployment.

### `Components/`
Higher-level management components.

- **TransceiverCommsManager**: Handles the high-level logic of when to transmit beacons vs. telemetry, and manages the state machine for the transceiver.
- **TransceiverConfigurationManager**: Interfaces with the hardware driver to send configuration commands for setting radio frequency, power modes, etc.

### `keys_template/`
Provides the mechanism for users to supply their radio's parameters without hardcoding them into the open-source scaffolding. Users populate a `TransceiverConfig.hpp` file based on the provided example.

## Dependencies

- **F' (F Prime)**: The underlying flight software framework. Greater than V4.
- **C++11/17**: For component implementations.
- **FPP**: For structural modeling and connection definitions.
