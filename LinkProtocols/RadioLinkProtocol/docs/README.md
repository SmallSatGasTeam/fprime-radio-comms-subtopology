# RadioLinkProtocol

This directory holds the files that wire a custom radio link protocol into F' deployments.

The default F' link protocols (e.g., CCSDS) frame payloads in a standardised header/payload/trailer layout. When a deployment uses radio hardware that expects a different on-the-wire format, a custom framer/deframer/frame-detector is needed. This module provides the F' component scaffolding for that — the actual framing logic is intentionally left as a stub for operators to fill in against their hardware's datasheet.

See the `RadioFramer/`, `RadioDeframer/`, and `RadioFrameDetector/` directories and their `docs/` folders for component-level details.
