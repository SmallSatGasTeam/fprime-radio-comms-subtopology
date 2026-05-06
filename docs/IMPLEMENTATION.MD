# Implementation Strategy: Structural Scaffolding

This project ships only the F' structural scaffolding for a radio communication stack — no radio-specific framing algorithm, polynomial, sync word, or vendor-specific command set is included. Operators provide those values through a configuration injection layer.

### Open-source scaffolding

- **Component definitions (.fpp)**: ports, commands, events, and telemetry for the communication stack.
- **Subtopology connections (.fpp)**: how framing components connect to each other and to standard F' services (`ComQueue`, `FprimeRouter`, etc.).
- **Management logic (.cpp)**: non-radio-specific management logic — beacon state machine, I2C transaction sequencing.
- **Deployment example**: a full F' deployment showing the wiring.

### Stubbed bodies

The framing-related bodies are intentional pass-throughs:
- Framing / deframing (preamble, sync word, length field handling).
- Link-layer encapsulation (AX.25 helpers).
- Frame check / CRC computation.

### Injection via `keys_template/`

1. **Header configuration**: copy `keys_template/TransceiverConfig.hpp.example` to `keys_template/TransceiverConfig.hpp` (gitignored) and fill in the values from your radio's datasheet.
2. **Stub bodies**: implement the marked stubs in the `.cpp` files using your radio's documentation.
