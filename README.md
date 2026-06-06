# Radio Communication Stack for F' (Structural Reference)

![Image](Assets/RadioProtocol_FramingSubtopologyTopology.png)
> Image generated using `fprime-util visualize` inside `Deployments/RadioDeployment/Top/`.

## Overview

An open-source, F' (F Prime) structural scaffolding for a generic radio communication stack. It wires custom framer/deframer/frame-detector components into the standard F' communication services (`ComQueue`, `FprimeRouter`, `BufferManager`, `ComStub`) so an operator can drop in their own licensed framing logic without reinventing the topology.

The framing logic itself is intentionally a stub — see [docs/USAGE_GUIDE.md](docs/USAGE_GUIDE.md) for how to implement the marked stub bodies against your hardware's datasheet.

## Key Features

- **F' architectural reference** demonstrating a complete, modular communication stack using standard F' components.
- **Reusable FPP subtopology** (`RadioProtocol`) that can be imported into any F' deployment.
- **Management component interfaces**: `TransceiverCommsManager` (downlink/beacon gateway) and `TransceiverConfigurationManager` (I2C radio configuration) — F' interfaces with stubbed behavior for operators to implement.

## Getting Started

### Prerequisites

- Git, a C/C++ toolchain (CMake + GCC/Clang), and Python 3 (3.8–3.12).
- Familiarity with the [F' (F Prime) framework](https://fprime.jpl.nasa.gov/latest/docs/getting-started/).

### Environment Setup

This project vendors F' as a git submodule and uses a project-local Python virtual environment for the F' tooling (`fprime-util`, `fprime-gds`, the FPP autocoder).

```bash
# 1. Clone with submodules (or initialize them in an existing clone)
git submodule update --init --recursive

# 2. Create and activate a virtual environment
python3 -m venv fprime-venv
source fprime-venv/bin/activate

# 3. Install the pinned F' toolchain
pip install -U pip setuptools wheel
pip install -r fprime/requirements.txt
```

Verify the toolchain by building the example deployment:

```bash
cd Deployments/RadioDeployment
fprime-util generate
fprime-util build
```

Re-activate the environment in new shells with `source fprime-venv/bin/activate` (and `deactivate` to exit). The `fprime-venv/` directory is git-ignored — do not commit it.

### Using the Comm Stack in Your Project

1. **Import the subtopology**: in your deployment's `topology.fpp`, import the `RadioProtocol` subtopology.
2. **Implement the stubs**: fill in the marked stub bodies in the `.cpp` files under `LinkProtocols/RadioLinkProtocol/` (framing/deframing/frame detection) and `Components/` (beacon and I2C logic), using your radio's datasheet.

## Repository Structure

- `Components/` — high-level management component interfaces (beacon gateway, I2C configuration).
- `LinkProtocols/` — framer / deframer / frame-detector scaffolding.
- `Deployments/` — example F' deployment demonstrating stack integration.

## Documentation

- [docs/USAGE_GUIDE.md](docs/USAGE_GUIDE.md) — step-by-step integration guide.
- [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) — architectural specification.
- [LinkProtocols/RadioLinkProtocol/README.md](LinkProtocols/RadioLinkProtocol/README.md) — framing stack details.
