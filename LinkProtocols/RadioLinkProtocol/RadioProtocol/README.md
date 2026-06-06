# RadioProtocol Subtopology

An F' subtopology that wires the radio link-protocol stack into the standard F'
communication services. It instantiates and connects:

- `Svc.ComQueue`, `Svc.FrameAccumulator`, `Svc.FprimeRouter`, `Svc.BufferManager`,
  and `Svc.ComStub` — standard F' communication services, and
- `RadioFramer`, `RadioDeframer`, `RadioFrameDetector` — this repo's link-protocol
  components (framing logic intentionally stubbed).

Import it into a deployment with `import RadioProtocol.Subtopology` and connect a
hardware driver (e.g. a UART driver) to the `comStub` instance.

## Files

- `RadioProtocol.fpp` — subtopology instances and the uplink/downlink connections.
- `RadioProtocolConfig/RadioProtocolConfig.fpp` — queue sizes, stack sizes, priorities,
  and buffer-manager bins.
- `RadioProtocolConfig/RadioProtocolSubtopologyConfig.{hpp,cpp}` — the memory allocator
  used by the subtopology.
- `PingEntries.hpp` / `SubtopologyTopologyDefs.hpp` — health-ping and topology-state
  definitions required by the autocoder.

See [`../README.md`](../README.md) and [`../../../docs/USAGE_GUIDE.md`](../../../docs/USAGE_GUIDE.md)
for the integration guide.
