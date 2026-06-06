# Subtopology `RadioProtocol`

`RadioProtocol` is an F' subtopology that connects the radio link-protocol components
(`RadioFramer`, `RadioDeframer`, `RadioFrameDetector`) to the standard F' communication
services and exposes a single hardware-driver attachment point (`comStub`).

## Instances

| Instance | Component | Role |
|---|---|---|
| `comQueue` | `Svc.ComQueue` | Queues outgoing events/telemetry/file/downlink packets |
| `RadioFramer` | `RadioLinkProtocol.RadioFramer` | Frames outgoing packets (stubbed) |
| `RadioDeframer` | `RadioLinkProtocol.RadioDeframer` | Deframes incoming frames (stubbed) |
| `FrameAccumulator` | `Svc.FrameAccumulator` | Accumulates byte-stream input into frames using `RadioFrameDetector` |
| `fprimeRouter` | `Svc.FprimeRouter` | Routes deframed F' packets to command/telemetry/file subsystems |
| `commsBufferManager` | `Svc.BufferManager` | Supplies buffers for the stack |
| `comStub` | `Svc.ComStub` | Attachment point for the hardware byte-stream driver |

## Data Paths

- **Downlink**: `comQueue → RadioFramer → comStub → driver`
- **Uplink**: `driver → comStub → FrameAccumulator → RadioDeframer → fprimeRouter`

## Integration

Import with `import RadioProtocol.Subtopology`, connect a hardware driver to `comStub`, and
connect your command dispatcher / telemetry / file downlink to `fprimeRouter` and `comQueue`.
See [`../../../../docs/USAGE_GUIDE.md`](../../../../docs/USAGE_GUIDE.md) for the full guide.

Configuration constants (queue sizes, stack sizes, priorities, buffer bins) live in
`RadioProtocolConfig/RadioProtocolConfig.fpp`.

## Change Log

| Date | Description |
|---|---|
| 2026-06-05 | Rewritten to describe the actual subtopology (replaced generator boilerplate) |
