# Radio Link Protocol (F' Structural Reference)

This directory contains the F' scaffolding for a generic radio communication stack.

The framing logic itself is intentionally a stub — operators provide their own implementation in the following components against their hardware's datasheet:

- `RadioFramer`
- `RadioDeframer`
- `RadioFrameDetector`

The repository is useful as a reference for:
- How to structure a framing/deframing stack in F'.
- How to connect `ComQueue`, `FprimeRouter`, `FrameAccumulator`, and `BufferManager` in a subtopology.
- How to integrate a custom link protocol with standard F' components.

## Project Structure

- `RadioFramer/` — F' component for wrapping F' packets into radio frames.
- `RadioDeframer/` — F' component for stripping headers and routing packets to the F' system.
- `RadioFrameDetector/` — Implementation of `Svc::FrameDetector` for identifying frame boundaries in a byte stream.
- `RadioProtocol/` — Subtopology definition connecting all framing components.

## Helpful resources

- F' [custom framer guide](https://fprime.jpl.nasa.gov/latest/docs/reference/communication-adapter-interface).
- F' documentation: [https://fprime.jpl.nasa.gov](https://fprime.jpl.nasa.gov)
