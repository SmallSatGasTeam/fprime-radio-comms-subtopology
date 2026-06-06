// ======================================================================
// \title  RadioFrameDetector.cpp
// \brief  Frame detector for the radio link protocol
// ======================================================================

#include "RadioFrameDetector.hpp"

namespace RadioLinkProtocol {

    RadioFrameDetector::RadioFrameDetector()
        : Svc::FrameDetector() {
    }

    RadioFrameDetector::~RadioFrameDetector() {}

    RadioFrameDetector::Status RadioFrameDetector::detect(
        const Types::CircularBuffer& ring,
        FwSizeType& frameSize
    ) const {
        // Stub: any available data is treated as one frame. Implement detection here.
        const FwSizeType available = ring.get_allocated_size();
        if (available > 0) {
            frameSize = available;
            return Svc::FrameDetector::FRAME_DETECTED;
        }

        return Svc::FrameDetector::MORE_DATA_NEEDED;
    }

} // namespace RadioLinkProtocol
