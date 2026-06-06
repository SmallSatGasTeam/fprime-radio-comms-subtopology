# Integration and Usage Guide

This guide explains how to take the open-source F' structural scaffolding for the radio communication stack and integrate it into your own F' (F Prime) flight software project, including how to implement its framing logic.

## Prerequisites

- A working F' project (version > v4).
- Datasheets / documentation for your radio so you can implement the framing logic and I2C commands.

---

## Step 1: Add the Submodule

First, add this repository to your F' project (e.g., as a git submodule inside a `lib/` or `Components/` directory).

```bash
git submodule add https://github.com/SmallSatGasTeam/fprime-radio-comms-subtopology.git lib/fprime-radio-comms-subtopology
```

In your project's top-level `CMakeLists.txt` or `project.cmake`, include the directory so F' knows to build it:

```cmake
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/lib/fprime-radio-comms-subtopology/")
```

---

## Step 2: Implement the Stub Bodies

The framing logic in this scaffolding is intentionally stubbed. Implement each stub against your radio's documentation.

### 1. `RadioFramer.cpp`
Located at `LinkProtocols/RadioLinkProtocol/RadioFramer/RadioFramer.cpp`. Replace the pass-through in `frameData()` with your framing logic (preamble, sync word, length field, CRC), writing each field into the allocated buffer through its serializer.

### 2. `RadioDeframer.cpp`
Located at `LinkProtocols/RadioLinkProtocol/RadioDeframer/RadioDeframer.cpp`. Implement `dataIn_handler` to strip your link headers and update the `FrameContext` with the stripped offset so buffer pointers can be restored on the return path.

### 3. `RadioFrameDetector.cpp`
Located at `LinkProtocols/RadioLinkProtocol/RadioFrameDetector/RadioFrameDetector.cpp`. Replace the pass-through `detect()` with your sync-word / length-field scanning logic.

> The `Components/` management components (`TransceiverCommsManager`, `TransceiverConfigurationManager`) are likewise stubbed — implement their handlers if you need beacon management or I2C radio configuration. Radio-specific values (sync words, polynomials, callsigns, I2C command bytes) are supplied by your implementation; this repository does not ship them.

---

## Step 3: Import the Subtopology

Once the logic is implemented, wire the entire communication stack into your deployment using FPP. In your deployment's `topology.fpp` file, import the RadioProtocol subtopology:

```fpp
import RadioProtocol.Subtopology
```

Instantiate the required driver component for your hardware (e.g., a UART driver):

```fpp
instance LinuxUartDriver: Drv.LinuxUartDriver base id 0x10000020
```

Connect the driver to the framing stack. The subtopology exposes a `comStub` component that acts as the interface point between the framing logic and the hardware driver.

```fpp
connections RadioStack {
    # Driver buffer allocations
    LinuxUartDriver.allocate   -> RadioProtocol.commsBufferManager.bufferGetCallee
    LinuxUartDriver.deallocate -> RadioProtocol.commsBufferManager.bufferSendIn

    # Receive path (Uplink: Antenna -> Driver -> Stub -> Deframer)
    LinuxUartDriver.recv                      -> RadioProtocol.comStub.drvReceiveIn
    RadioProtocol.comStub.drvReceiveReturnOut -> LinuxUartDriver.recvReturnIn

    # Transmit path (Downlink: Framer -> Stub -> Driver -> Antenna)
    RadioProtocol.comStub.drvSendOut      -> LinuxUartDriver.send
    LinuxUartDriver.ready                 -> RadioProtocol.comStub.drvConnected
}
```

---

## Step 4: Connect F' Services

Finally, connect your deployment's command dispatcher, telemetry sender, and file downlink to the subtopology's router and queue.

```fpp
connections RadioProtocol_CdhCore {
    # Core events and telemetry to communication queue
    CdhCore.events.PktSend -> RadioProtocol.comQueue.comPacketQueueIn[RadioProtocol.Ports_ComPacketQueue.EVENTS]
    CdhCore.tlmSend.PktSend -> RadioProtocol.comQueue.comPacketQueueIn[RadioProtocol.Ports_ComPacketQueue.TELEMETRY]

    # Router to Command Dispatcher
    RadioProtocol.fprimeRouter.commandOut -> CdhCore.cmdDisp.seqCmdBuff
    CdhCore.cmdDisp.seqCmdStatus -> RadioProtocol.fprimeRouter.cmdResponseIn
}
```
