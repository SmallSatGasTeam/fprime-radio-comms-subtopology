# AX25::AX25Framing

Utility module providing AX.25 UI-frame construction and detection for use by link protocol framers and deframers.

## Usage Examples

`AX25Framing` is not an fprime component. It is a stateless utility library consumed by framer and deframer components (e.g. `RadioFramer`, `RadioDeframer`) that need to build or strip an AX.25 frame inside a larger radio packet.

### Framing (outbound)

1. The caller computes the total required buffer size using `AX25::frameSize(payloadSize)`.
2. The caller allocates a buffer and writes any radio-layer headers into it (preamble, sync word, length byte, etc.) using an `Fw::ExternalSerializeBufferWithMemberCopy` serializer.
3. The caller passes the serializer, a pointer to the buffer base, the byte offset where the AX.25 frame will start, the payload pointer, and the payload size to `AX25::writeFrame()`.
4. `writeFrame()` serializes the full AX.25 UI frame — preamble, start flag, address fields, control, PID, info field, FCS, end flag, and postamble — directly into the serializer in the correct order.
5. The caller appends any remaining radio-layer trailer bytes (e.g. an outer CRC) and dispatches the buffer.

### Deframing (inbound)

1. The caller strips the radio-layer header, obtaining a pointer to the start of the AX.25 frame and its size.
2. The caller passes those to `AX25::stripFrame()`, which validates the frame and returns the byte offset and size of the embedded info field.
3. If `stripFrame()` returns `true`, the caller advances its buffer pointer by `infoOffset` and sets the buffer size to `infoSize` to expose only the payload.
4. If `stripFrame()` returns `false`, the buffer is not AX.25-wrapped and should be handled as a raw payload.

## Technical Details

### Frame Structure

`AX25::writeFrame()` produces the following byte sequence, written directly into the provided serializer:

| Field | Value | Length |
| --- | --- | --- |
| Preamble | `0x7E` × 8 | 8 bytes |
| Start Flag | `0x7E` | 1 byte |
| Destination Callsign | `"CQ    "` (AX.25 shifted) | 6 bytes |
| Destination SSID | `0xE0` | 1 byte |
| Source Callsign | `"NOCALL "` (AX.25 shifted) | 6 bytes |
| Source SSID | `0xE1` | 1 byte |
| Control | `0x03` (UI frame) | 1 byte |
| PID | `0xF0` (no layer 3) | 1 byte |
| Info | Caller-supplied payload | `size` bytes |
| FCS | CRC16-CCITT over address + control + PID + info | 2 bytes |
| End Flag | `0x7E` | 1 byte |
| Postamble | `0x7E` × 3 | 3 bytes |

Total overhead (`FRAME_OVERHEAD`): **31 bytes** (`HEADER_LEN` = 25 + `TRAILER_LEN` = 6).

### FCS Computation

The Frame Check Sequence is computed using the CRC16-CCITT algorithm (polynomial `0x1021`, initial value `0xFFFF`). It covers the 14-byte address field, the 2-byte control/PID field, and the info payload. The computation reads from the already-serialized bytes in the output buffer using the `buffer_base` + `ax25_offset` pointers passed by the caller, so no intermediate copy is needed.

### AX.25 Address Encoding

AX.25 callsigns are left-shifted by one bit per the AX.25 standard. The destination callsign is `CQ` (broadcast) padded to 6 characters; the source callsign is `NOCALL` padded to 6 characters.

## API

### `AX25::frameSize(U32 payloadSize) -> U32`
Returns the total number of bytes an AX.25 frame will occupy for the given payload size. Equal to `FRAME_OVERHEAD + payloadSize`.

### `AX25::writeFrame(...) -> Fw::SerializeStatus`
Writes a complete AX.25 UI frame into the serializer. Returns `Fw::FW_SERIALIZE_OK` on success, or the first failing `Fw::SerializeStatus` on buffer overflow.

| Parameter | Type | Description |
| --- | --- | --- |
| `serializer` | `Fw::ExternalSerializeBufferWithMemberCopy&` | Serializer positioned at the start of the AX.25 frame region |
| `buffer_base` | `const U8*` | Pointer to byte 0 of the output buffer (for in-place FCS computation) |
| `ax25_offset` | `U32` | Byte offset from `buffer_base` where the AX.25 frame starts |
| `data` | `const U8*` | Pointer to the info-field (payload) bytes |
| `size` | `U32` | Number of payload bytes |

### `AX25::stripFrame(...) -> bool`
Checks whether a buffer starts with a valid AX.25 UI frame and returns the location and size of the embedded info field. Returns `true` if a valid AX.25 wrapper was detected, `false` otherwise.

| Parameter | Type | Description |
| --- | --- | --- |
| `frame` | `const U8*` | Pointer to the first byte of the AX.25 frame |
| `frameSize` | `U32` | Total number of bytes in the AX.25 frame |
| `infoOffset` | `U32&` | [out] Byte offset from `frame` to the start of the info field (`HEADER_LEN` = 25 on success) |
| `infoSize` | `U32&` | [out] Size of the info field in bytes (`frameSize - HEADER_LEN - TRAILER_LEN` on success) |

## Requirements

| Name | Description | Validation |
| --- | --- | --- |
| Frame Construction | Produce a correctly-structured AX.25 UI frame with preamble, address, control, PID, info, FCS, and postamble. | Unit test |
| FCS Correctness | CRC16-CCITT FCS must be computed over the address + control + PID + info bytes. | Unit test |
| Size Calculation | `frameSize()` must return exactly `FRAME_OVERHEAD + payloadSize`. | Unit test |
| Frame Detection | `stripFrame()` must return `true` only when `frameSize >= FRAME_OVERHEAD` and `frame[0] == 0x7E`. | Unit test |
| Info Extraction | `stripFrame()` must set `infoOffset = HEADER_LEN` and `infoSize = frameSize - FRAME_OVERHEAD` on success. | Unit test |
| No Dynamic Allocation | The module must not perform heap allocation; it writes into the caller-supplied serializer only. | Code review |

## Change Log

| Date | Description | Author |
| --- | --- | --- |
| 2026-03-26 | Initial documentation | xtilloo |
| 2026-03-26 | Added `stripFrame()` for deframer use; added deframing usage section and updated requirements | xtilloo |
