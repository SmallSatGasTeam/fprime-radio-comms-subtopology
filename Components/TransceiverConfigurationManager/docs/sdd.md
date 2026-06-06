# Component::TransceiverConfigurationManager

> **Status: structural stub.** This component defines the F' interface for configuring a
> radio transceiver over I2C, but the configuration logic is intentionally **not
> implemented**. Only the health-ping passthrough does real work; the `configureSettings`
> command and all I2C helpers are stubs. There are no unit tests in this repository.

Active component intended to configure radio parameters (frequency, callsigns, power mode,
etc.) over I2C. As shipped, it is a structural reference only.

## Interface

### Ports

| Name | Type | Direction | Purpose |
|---|---|---|---|
| `i2cWriteRead` | `Drv.I2cWriteRead` | output | I2C write-then-read transactions |
| `i2cRead` | `Drv.I2c` | output | I2C read |
| `i2cWrite` | `Drv.I2c` | output | I2C write |
| `allocate` / `deallocate` | `Fw.BufferGet` / `Fw.BufferSend` | output | Buffer allocation / deallocation |
| `pingIn` / `pingOut` | `Svc.Ping` | async input / output | Health ping |
| standard ports | — | — | time, command reg/recv/resp, text event, event, telemetry, param |

### Commands

| Command | Arguments | Description |
|---|---|---|
| `configureSettings` | none | (Stub) intended to run the full I2C radio-configuration sequence |

### Events (declared in FPP; not emitted by the stub)

| Name | Severity | Description |
|---|---|---|
| `UHFSuccess` | ACTIVITY_HI | I2C transaction succeeded |
| `UHFAddressFailure` / `UHFWriteError` / `UHFReadError` / `UHFOpenError` / `UHFOtherError` | WARNING_HI | I2C failure conditions |
| `MemoryAllocationFailed` | WARNING_LO | Buffer allocation failure |
| `debuggingEvent(string)` | ACTIVITY_HI | Generic debug output |

### Telemetry (declared in FPP; not updated by the stub)

| Name | Type | Description |
|---|---|---|
| `temperature` | U8 | Transceiver internal temperature |
| `uptime` | U8 | Time since transceiver power-on |
| `lowPowerMode` | bool | Low-power mode flag |

## Implemented behavior (as shipped)

| Handler | Behavior |
|---|---|
| `pingIn_handler` | Echoes the ping to `pingOut` |
| `configureSettings_cmdHandler` | Responds `OK`; no I2C activity |
| `configureSettings` / `sendConfigCommand` / `sendI2cCmd` / `checkI2cStatus` / `parseBuffer` | No-op / empty-return stubs |

## Integration notes

To make this functional: implement the I2C sequence in `configureSettings`, build
transactions in `sendI2cCmd`, validate status in `checkI2cStatus`, and emit the declared
events/telemetry. Radio-specific command bytes and the I2C address must be supplied by the
operator (e.g. as component parameters or constants) — this repo no longer ships a
configuration-injection layer.

## Change Log

| Date | Description |
|---|---|
| 2026-03-29 | Initial component creation and documentation |
| 2026-06-05 | Documented as structural stub; removed unimplemented behavior, config-injection, and unit-test claims |
