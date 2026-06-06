# RadioLinkProtocol::RadioDeframer

Component for deframing inbound radio packets and identifying nested F' protocol packets.

The deframing logic itself is intentionally a stub (a pass-through) in this open-source scaffolding. Operators supply their deframer implementation in the `dataIn_handler` body in `RadioDeframer.cpp`, against their radio's datasheet. The steps below describe a typical target implementation, not the shipped stub.

## Architecture

`RadioDeframer` sits between `Svc::FrameAccumulator` (which produces complete byte-aligned frames from the byte-stream driver) and `Svc::FprimeRouter` (which dispatches F' packets to command, telemetry, file, and log subsystems).

A typical implementation will:
1. Validate the radio's frame check (e.g., CRC) using your radio's frame parameters.
2. Strip the radio framing to expose the link-layer payload.
3. Optionally delegate to one or more link-layer protocol modules via their `stripFrame()` helpers. Each link-layer module is responsible for detecting and stripping its own framing; the deframer only acts on the boolean result.
4. Update the `FrameContext` with the identified APID and the total stripped offset so the buffer pointer can be restored on the return path.
5. Forward the inner F' payload to `Svc::FprimeRouter`.

## Memory Safety

Because the deframer modifies the `Fw::Buffer` pointer to "strip" headers (zero-copy), it must restore the original pointer on the return path:
- The total stripped offset is stored in `FrameContext::comQueueIndex`.
- In `dataReturnIn_handler`, that offset is subtracted from the data pointer before the buffer is returned to the allocator.

## Port Descriptions

| Name | Description |
| --- | --- |
| input: dataIn | Receives complete frames from `Svc::FrameAccumulator`. |
| output: dataOut | Forwards inner F' payloads to `Svc::FprimeRouter`. |
| output: dataReturnOut | Returns ownership of buffers back to the driver/allocator. |
| input: dataReturnIn | Receives back ownership of sent buffers from downstream components. |

## Events

| Name | Description | Severity |
| --- | --- | --- |
| InvalidChecksum | The radio-level frame check failed; the frame was dropped. | WARNING_HI |

## Requirements

| Name | Description | Validation |
| --- | --- | --- |
| Frame Validation | Reject frames whose computed check value does not match the trailer. | Operator-implemented |
| Memory Management | Use `FrameContext` to restore original buffer pointers on the return path. | Operator-implemented |
| Type Safety | Default to `FW_PACKET_UNKNOWN` if no link-layer wrapper recognises the payload. | Operator-implemented |
