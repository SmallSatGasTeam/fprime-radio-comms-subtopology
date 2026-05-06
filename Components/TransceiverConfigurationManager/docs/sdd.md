# Component::TransceiverConfigurationManager

Active component responsible for I2C configuration of the radio transceiver and management of pipe mode state. Configures radio parameters (e.g., frequency, callsigns) and enables transparent UART-to-radio bridging in pipe mode.

## Usage Examples

### Typical Usage
1. On startup or when radio reconfiguration is needed, invoke the `configureSettings` command
2. The component reads configuration parameters and sends I2C commands to the transceiver
3. Once configured, pipe mode is enabled with a long timeout, making UART transparent
4. The component remains idle until reconfiguration is requested or pipe mode expires
5. Other components query pipe mode state via the `getPipeMode` sync port
6. Health pings are accepted and responded to for system health monitoring

## Port Descriptions

| Name | Type | Direction | Purpose |
|---|---|---|---|
| `i2cRead` | `Drv.I2c` | output | I2C read transactions to transceiver |
| `i2cWrite` | `Drv.I2c` | output | I2C write transactions to transceiver |
| `i2cReadWrite` | `Drv.I2cWriteRead` | output | I2C write-then-read transactions |
| `getPipeMode` | sync input | input | Query current pipe mode state (getter) |
| `setPipeMode` | sync input | input | Enable/disable pipe mode via I2C (setter) |
| `allocate` | `Fw.BufferGet` | output | Buffer allocation for I2C transactions |
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

The component is passive and stateless with respect to mission logic. It maintains internal pipe mode state:

| Name | Description |
|---|---|
| Idle | Waiting for reconfiguration or pipe mode coordination requests |
| Configuring | Executing I2C configuration sequence (transient) |
| Pipe Mode Enabled | Transparent UART-to-radio bridging active |
| Pipe Mode Disabled | Radio requires explicit commands for each transmission |

## Commands

| Command | Arguments | Description |
|---|---|---|
| `configureSettings` | none | Execute full radio configuration sequence and enable pipe mode |

## Functions

| Name | Description |
|---|---|
| `configureSettings_cmdHandler()` | Initiates the radio configuration sequence |
| `getPipeMode_handler()` | Sync port handler returning current pipe mode state (bool) |
| `setPipeMode_handler()` | Sync port handler to set pipe mode via I2C command |
| `pingIn_handler()` | Health ping input, forwards to pingOut |
| `sendI2cCmd()` | Allocates buffer, serializes I2C command, sends via i2cRead/i2cWrite/i2cReadWrite |
| `checkI2cStatus()` | Validates I2C status, logs appropriate event |
| `sendConfigCommand()` | Sends a single configuration command via I2C |
| `parseBuffer()` | Parses I2C response for success/failure (checks for expected success prefix) |

## Events

| Name | Severity | Description |
|---|---|---|
| `UHFSuccess` | ACTIVITY_HI | I2C transaction succeeded |
| `UHFAddressFailure` | WARNING_HI | Invalid I2C address for transceiver |
| `UHFWriteError` | WARNING_HI | I2C write operation failed |
| `UHFReadError` | WARNING_HI | I2C read operation failed |
| `UHFOpenError` | WARNING_HI | Failed to open I2C device |
| `UHFOtherError` | WARNING_HI | Other unspecified I2C error |
| `MemoryAllocationFailed` | WARNING_LO | Failed to allocate buffer for I2C transaction |
| `debuggingEvent(string)` | ACTIVITY_HI | Generic debug output |

## Telemetry

| Name | Type | Description |
|---|---|---|
| `temperature` | U8 | Transceiver internal temperature in degrees Celsius |
| `uptime` | U8 | Time elapsed since transceiver last powered on |
| `lowPowerMode` | bool | Whether transceiver is in low power mode |

## Internal Constants

| Name | Value | Description |
|---|---|---|
| `ADDRESS` | 0xXX | I2C address of transceiver (injected via config) |
| `WRITE_*` | - | Radio-specific write command bytes |
| `READ_*` | - | Radio-specific read command bytes |

## Unit Tests

| Name | Description | Status |
|---|---|---|
| `testGetPipeModeInitiallyFalse` | Verify getPipeMode sync port returns false on initialization | ✓ PASS |
| `testPingPassthrough` | Verify pingIn forwarded correctly to pingOut | ✓ PASS |

## Requirements

| Requirement | Description | Validation |
|---|---|---|
| CFG-1 | Configure transceiver frequency via I2C | Covered by `configureSettings` command implementation |
| CFG-2 | Configure transceiver callsigns via I2C | Covered by `configureSettings` command implementation |
| CFG-3 | Enable pipe mode for transparent UART bridging | Covered by `setPipeMode` handler |
| CFG-4 | Report I2C transaction status via events | Covered by event logging in `checkI2cStatus` |
| CFG-5 | Health ping support | Covered by `pingIn_handler` test |

## Change Log

| Date | Description |
|---|---|
| 2026-03-29 | Initial component creation and documentation |
