# Component::TransceiverCommsManager

> **Status: structural stub.** This component defines the F' interface (ports, commands,
> events, telemetry) for a downlink gateway and beacon manager, but the behavior is
> intentionally **not implemented**. Only buffer return/deallocation and the health-ping
> passthrough do real work. The beacon state machine, beacon scheduling, and downlink
> routing described below are the integration points an operator fills in. There are no
> unit tests in this repository.

Active component intended to serve as the downlink gateway and beacon controller — all
outgoing data is meant to route through it, and it would manage periodic beacon
transmission. As shipped, it is a structural reference only.

## Interface

### Ports

| Name | Type | Direction | Purpose |
|---|---|---|---|
| `dataIn` | `Fw.BufferSend` | async input | Receives data buffers for downlink |
| `sendBuffer` | `Fw.BufferSend` | output | Forwards buffers to comQueue for framing |
| `bufferReturn` | `Fw.BufferSend` | async input | Returned buffers from comQueue after transmission |
| `setBeaconStateIn` | `Components.SetBeaconState` | async input | Programmatic beacon-state change |
| `schedIn` | `Svc.Sched` | async input | Rate-group tick (intended beacon timer) |
| `allocate` / `deallocate` | `Fw.BufferGet` / `Fw.BufferSend` | output | Buffer allocation / deallocation |
| `pingIn` / `pingOut` | `Svc.Ping` | async input / output | Health ping |
| standard ports | — | — | time, command reg/recv/resp, text event, event, telemetry, param |

### Commands

| Command | Arguments | Description |
|---|---|---|
| `sendData` | `string data` | (Stub) intended to send a test string through the framing stack |
| `setBeaconState` | `BeaconState newState` | (Stub) intended to change the beacon state |

### Events (declared in FPP; not emitted by the stub)

| Name | Severity | Description |
|---|---|---|
| `beaconStateChange(oldState, newState)` | ACTIVITY_HI | Beacon state transition |
| `transmitDataSuccess` | ACTIVITY_HI | Data sent to framing stack |
| `transmitDataFailure` | WARNING_HI | Failed to send data |
| `MemoryAllocationFailed` | WARNING_LO | Buffer allocation failure |

### Telemetry (declared in FPP; not updated by the stub)

| Name | Type | Description |
|---|---|---|
| `beaconState` | `BeaconState` enum | Current beacon state |
| `packetsSent` | U32 | Packets sent to the framing stack |

### `BeaconState` enum

`SILENT`, `ACTIVE_BEACON`, `LOW_POWER`, `DOWNLINKING`, `DISABLED` — defined in the FPP as
the intended states for an operator-implemented beacon state machine.

## Implemented behavior (as shipped)

| Handler | Behavior |
|---|---|
| `dataIn_handler` | Returns the incoming buffer immediately (no routing) to avoid leaks |
| `bufferReturn_handler` | Deallocates the returned buffer if valid |
| `pingIn_handler` | Echoes the ping to `pingOut` |
| `sendData_cmdHandler` / `setBeaconState_cmdHandler` | Respond `OK`; no other action |
| `setBeaconStateIn_handler` / `schedIn_handler` / `forwardToFramingStack` | No-op stubs |

## Integration notes

To make this functional: implement the beacon state machine in `schedIn_handler`, route
`dataIn` to `sendBuffer` via `forwardToFramingStack`, and emit the events/telemetry
declared above.

## Change Log

| Date | Description |
|---|---|
| 2026-03-29 | Initial component creation and documentation |
| 2026-06-05 | Documented as structural stub; removed unimplemented behavior, config-injection, and unit-test claims |
