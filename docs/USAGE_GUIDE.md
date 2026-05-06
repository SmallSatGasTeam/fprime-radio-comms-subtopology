# Integration and Usage Guide

This guide explains how to take the open-source F' structural scaffolding for the radio communication stack and integrate it into your own F' (F Prime) flight software project, including how to inject your radio's parameters and framing logic.

## Prerequisites

- A working F' project (version > v4).
- Datasheets / documentation for your radio so you can fill in the framing parameters and I2C commands.

---

## Step 1: Add the Submodule

First, add this repository to your F' project (e.g., as a git submodule inside a `lib/` or `Components/` directory).

```bash
git submodule add https://github.com/Xtilloo/fprime-radio-uhf-comms.git lib/fprime-radio-uhf-comms
```

In your project's top-level `CMakeLists.txt` or `project.cmake`, include the directory so F' knows to build it:

```cmake
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/lib/fprime-radio-uhf-comms/")
```

---

## Step 2: Inject Configuration

This project abstracts radio-specific values (sync words, polynomials, callsigns, I2C command strings, etc.) into a single configuration object.

1. Locate the template at `keys_template/TransceiverConfig.hpp.example`.
2. Copy it into a **private, non-public** directory within your own project (e.g., `Config/TransceiverConfig.hpp`).
3. Fill in every field using the values from your radio's datasheet.
4. Ensure your project's CMake configuration includes the directory containing your `TransceiverConfig.hpp` in its include path so the components can find it.

---

## Step 3: Implement the Stub Bodies

The framing logic in this scaffolding is intentionally stubbed. Implement each stub against your radio's documentation.

### 1. `RadioFramer.cpp`
Located at `LinkProtocols/RadioLinkProtocol/RadioFramer/RadioFramer.cpp`. Replace the pass-through `memcpy` in `frameData()` with your framing logic.

### 2. `RadioDeframer.cpp`
Located at `LinkProtocols/RadioLinkProtocol/RadioDeframer/RadioDeframer.cpp`. Implement `dataIn_handler` and `validateFrame`. Update the `FrameContext` with the stripped offset so buffer pointers can be restored on the return path.

### 3. `RadioFrameDetector.cpp`
Located at `LinkProtocols/RadioLinkProtocol/RadioFrameDetector/RadioFrameDetector.cpp`. Replace the pass-through `detect()` with your sync-word / length-field scanning logic.

---

## Step 4: Import the Subtopology

Once the logic is implemented, you can easily wire the entire communication stack into your deployment using FPP.

In your deployment's `topology.fpp` file, import the RadioProtocol subtopology:

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
    LinuxUartDriver.ready                     -> RadioProtocol.comStub.drvConnected 
}
```

---

## Step 5: Connect F' Services

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

## Step 6: Define `TRANSCEIVER_CONFIG`

In your deployment's C++ topology setup code (e.g., `Topology.cpp`), define the global configuration object that the subtopology expects:

```cpp
#include "Config/TransceiverConfig.hpp"

namespace YourDeployment {
    // This definition is required by the RadioProtocol subtopology
    TransceiverConfig::Config TRANSCEIVER_CONFIG;
}
```

(Ensure you initialize or load values into this object before `setupTopology()` is called if they are dynamic, though statically compiling them into the struct is standard).
