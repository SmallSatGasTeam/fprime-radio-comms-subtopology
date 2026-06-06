// ======================================================================
// \title  RadioFrameDetector.hpp
// \brief  Frame detector for the radio link protocol
// ======================================================================

#ifndef RADIO_FRAME_DETECTOR_HPP
#define RADIO_FRAME_DETECTOR_HPP

#include <Svc/FrameAccumulator/FrameDetector.hpp>

namespace RadioLinkProtocol {

    class RadioFrameDetector : public Svc::FrameDetector {
        public:
            //! Constructor
            RadioFrameDetector();

            ~RadioFrameDetector();

            Status detect(const Types::CircularBuffer& ring, FwSizeType& frameSize) const override;
    };
}

#endif
