# RadioLinkProtocol::RadioFramer

Passive component that wraps an outgoing payload into a frame for the configured radio and forwards it to the byte-stream driver.

The framing logic itself is intentionally a stub in this open-source scaffolding. Operators supply their framing implementation in the `frameData()` helper in `RadioFramer.cpp`, against their radio's datasheet. As shipped, the stub allocates an output buffer and copies the payload through unchanged.

## Port Descriptions

| Name | Direction | Description |
| --- | --- | --- |
| `dataIn` | input | Receives unframed payload buffers from the communication stack. |
| `dataReturnIn` | input | Receives back ownership of framed output buffers from the byte-stream driver after transmission. |
| `comStatusIn` | input | Receives ready/not-ready status from the downstream driver; propagated to `comStatusOut`. |
| `dataOut` | output | Sends the framed packet to the byte-stream driver. |
| `dataReturnOut` | output | Returns ownership of the original input buffer to the upstream sender. |
| `comStatusOut` | output | Forwards driver status upstream. |
| `bufferAllocate` | output | Requests a new buffer for the outgoing frame. |
| `bufferDeallocate` | output | Returns a used output buffer to the buffer manager after the driver is done with it. |

## Buffer Ownership

The component follows a zero-copy ownership model:
- The input `Fw::Buffer` from `dataIn` is never written to and is returned immediately to the caller via `dataReturnOut`.
- A separate output buffer is allocated, written, dispatched via `dataOut`, and later deallocated when it returns via `dataReturnIn`.

## Requirements

| Name | Description | Validation |
| --- | --- | --- |
| Buffer Ownership | Input buffers must be returned to the sender immediately; output buffers must be deallocated when returned by the driver. | Inspection |
| Allocation Guard | If the allocated buffer is smaller than required, it must be deallocated and the frame must be dropped without a crash. | Inspection |
