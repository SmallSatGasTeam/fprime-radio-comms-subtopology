# Component::TransceiverCommsManager

Active component serving as the downlink gateway and beacon state machine controller. Routes downlink data to the framing stack, manages beacon transmission timing across multiple power states, and coordinates with the radio configuration manager for pipe mode state.

## Usage Examples

### Typical Usage
1. Data from other components (e.g., FlightLogic) arrives on `dataIn` port
2. Component forwards data to framing stack via `sendBuffer` port
3. Beacon timer counts down on each `schedIn` rate group tick
4. When beacon interval expires in ACTIVE_BEACON or LOW_POWER states, automatic beacon is assembled and sent
5. FlightLogic or ground commands change beacon state via `setBeaconStateIn` port or `setBeaconState` command
6. Component transitions through beacon state machine: SILENT → ACTIVE_BEACON ↔ LOW_POWER ↔ DOWNLINKING ↔ DISABLED
7. Health pings are accepted and responded to for system health monitoring

### Data Flow
- **Downlink**: Component → framing stack (comQueue) → RadioFramer → comStub → hardware driver → antenna
- **Beacon**: Internally generated beacon message → same downlink path
- **Uplink**: Handled by deframing stack independently; not routed through this component

## Port Descriptions

| Name | Type | Direction | Purpose |
|---|---|---|---|
| `dataIn` | `Fw.BufferSend` | async input | Receives data buffers from components for downlink |
| `sendBuffer` | `Fw.BufferSend` | output | Forwards buffers to comQueue for framing |
| `bufferReturn` | `Fw.BufferSend` | async input | Returned buffers from comQueue after transmission |
| `setBeaconStateIn` | `Components.SetBeaconState` | async input | Allows FlightLogic to change beacon state programmatically |
| `schedIn` | `Svc.Sched` | async input | Rate group tick for beacon timer (e.g., 1 Hz) |
| `getPipeMode` | `Components.GetPipeMode` | sync output | Query pipe mode state from ConfigurationManager |
| `setPipeMode` | `Components.SetPipeMode` | sync output | Request pipe mode change from ConfigurationManager |
| `allocate` | `Fw.BufferGet` | output | Buffer allocation for beacon/data buffers |
| `deallocate` | `Fw.BufferSend` | output | Buffer deallocation |
| `pingIn` | `Svc.Ping` | async input | Health ping input |
| `pingOut` | `Svc.Ping` | output | Health ping response |
| `timeCaller` | time get | output | Request current time |
| `cmdRegOut` | command reg | output | Command registration |
| `cmdIn` | command recv | input | Receive commands |
| `cmdResponseOut` | command resp | output | Command responses |
| `logTextOut` | text event | output | Text event logging |
| `logOut` | event | output | Event logging |
| `tlmOut` | telemetry | output | Telemetry downlink |
| `prmGetOut` | param get | output | Parameter retrieval |
| `prmSetOut` | param set | output | Parameter setting |

## Component States

### Beacon State Machine

| State | Duration | Transitions | Description |
|---|---|---|---|
| SILENT | 30 minutes (DEPLOY_WAIT_ITER ticks) | → ACTIVE_BEACON on timer expiry | LEOP phase: no transmissions allowed |
| ACTIVE_BEACON | ∞ | ← LOW_POWER, ← DOWNLINKING, ← DISABLED | Normal periodic beacon at default interval (BEACON_INTERVAL ticks) |
| LOW_POWER | ∞ | ← ACTIVE_BEACON, → ACTIVE_BEACON | Reduced beacon rate (120 ticks) to conserve battery power |
| DOWNLINKING | ∞ | ← ACTIVE_BEACON / LOW_POWER | Beacon paused while data is being transmitted, resumes on bufferReturn |
| DISABLED | ∞ | ← ACTIVE_BEACON, → ACTIVE_BEACON | Beacon off by command |

### State Transitions

```
SILENT ──(30 min timer)──> ACTIVE_BEACON
ACTIVE_BEACON <──(low battery command)──> LOW_POWER
ACTIVE_BEACON <──(data arrival)──> DOWNLINKING
ACTIVE_BEACON <──(command)──> DISABLED
```

Transitions triggered by:
- `SILENT → ACTIVE_BEACON`: Internal 30-minute countdown, driven by `schedIn` tick counting
- `ACTIVE_BEACON ↔ LOW_POWER`: FlightLogic via `setBeaconStateIn` or ground command
- `ACTIVE_BEACON → DOWNLINKING`: Automatically on `dataIn` (data arrival)
- `DOWNLINKING → ACTIVE_BEACON`: Automatically on `bufferReturn` (transmission complete)
- `ACTIVE_BEACON ↔ DISABLED`: Ground command via `setBeaconState`

## Commands

| Command | Arguments | Description |
|---|---|---|
| `sendData` | `string data` | Test command: allocate buffer, serialize string, send to framing stack |
| `setBeaconState` | `BeaconState newState` | Change the beacon state machine state |

## Functions

| Name | Description |
|---|---|
| `sendData_cmdHandler()` | Allocates buffer, serializes test string, forwards to framing stack |
| `setBeaconState_cmdHandler()` | Invokes state transition |
| `dataIn_handler()` | On buffer arrival, transitions to DOWNLINKING (if in ACTIVE_BEACON/LOW_POWER), forwards to framing stack |
| `bufferReturn_handler()` | On buffer return, deallocates it, transitions back to ACTIVE_BEACON (if in DOWNLINKING) |
| `setBeaconStateIn_handler()` | Async port version of state change command |
| `schedIn_handler()` | Rate group tick; drives SILENT countdown, beacon interval timer, and sends beacon on timeout |
| `pingIn_handler()` | Health ping input, forwards to pingOut |
| `sendBeacon()` | Assembles beacon buffer with configured payload and sends to framing stack |
| `forwardToFramingStack()` | Common handler to send buffer on sendBuffer port and increment telemetry |
| `transitionBeaconState()` | Internal state transition, logs event, updates telemetry, resets tick counter |

## Events

| Name | Severity | Description |
|---|---|---|
| `beaconStateChange(oldState, newState)` | ACTIVITY_HI | Beacon state transitioned from one state to another |
| `transmitDataSuccess` | ACTIVITY_HI | Data successfully sent to framing stack |
| `transmitDataFailure` | WARNING_HI | Failed to send data (buffer allocation failure) |
| `MemoryAllocationFailed` | WARNING_LO | Failed to allocate buffer for beacon or data |

## Telemetry

| Name | Type | Description |
|---|---|---|
| `beaconState` | `BeaconState` enum | Current beacon state machine state |
| `packetsSent` | U32 | Total count of data packets and beacons sent to framing stack |

## Internal Constants

| Name | Value | Description |
|---|---|---|
| `SILENT_TICKS` | `DEPLOY_WAIT_ITER` (≈60 @ 1 Hz) | Ticks for 30-minute LEOP silent period |
| `BEACON_INTERVAL_TICKS` | `BEACON_INTERVAL` (≈6 @ 1 Hz, 6 sec) | Ticks between beacon transmissions in ACTIVE_BEACON |
| `LOW_POWER_BEACON_INTERVAL_TICKS` | 120 | Ticks between beacon transmissions in LOW_POWER (120 sec) |

## Unit Tests

| Name | Description | Status |
|---|---|---|
| `testInitialState` | Verify initial state is SILENT, packetsSent=0 | ✓ PASS |
| `testSilentToActiveBeacon` | Verify state transitions after DEPLOY_WAIT_ITER ticks | ✓ PASS |
| `testActiveBeaconSendsBeacon` | Verify beacon sent after BEACON_INTERVAL ticks in ACTIVE_BEACON | ✓ PASS |
| `testLowPowerBeaconInterval` | Verify 120-tick beacon interval in LOW_POWER state | ✓ PASS |
| `testDownlinkingPausesBeacon` | Verify dataIn triggers DOWNLINKING, sendBuffer called | ✓ PASS |
| `testBufferReturnRestoresActiveBeacon` | Verify bufferReturn transitions back to ACTIVE_BEACON | ✓ PASS |
| `testDisabledNoBeacon` | Verify no beacon sent when DISABLED | ✓ PASS |
| `testSendDataCommand` | Verify sendData command allocates, serializes, forwards | ✓ PASS |
| `testSetBeaconStateCommand` | Verify setBeaconState command fires event | ✓ PASS |

## Requirements

| Requirement | Description | Validation |
|---|---|---|
| COM-1 | Route downlink data to framing stack | Covered by `dataIn_handler` and `forwardToFramingStack` |
| COM-2 | Implement beacon state machine with 5 states | Covered by state enum and transition logic |
| COM-3 | 30-minute SILENT period at mission start | Covered by `SILENT_TICKS` countdown in `schedIn_handler` |
| COM-4 | Periodic beacon transmission in ACTIVE_BEACON | Covered by `BEACON_INTERVAL_TICKS` timer and `sendBeacon` |
| COM-5 | Support low power beacon mode | Covered by `LOW_POWER_BEACON_INTERVAL_TICKS` and state transitions |
| COM-6 | Pause beacon during downlink | Covered by `dataIn_handler` DOWNLINKING transition and `bufferReturn_handler` resume |
| COM-7 | Buffer ownership tracking | Covered by `bufferReturn_handler` deallocation |
| COM-8 | Health ping support | Covered by `pingIn_handler` test |

## Change Log

| Date | Description |
|---|---|
| 2026-03-29 | Initial component creation and documentation |
