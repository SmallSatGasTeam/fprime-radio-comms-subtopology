// ======================================================================
// \title  RadioFrameDetector.hpp
// \brief  Frame detector for the radio link protocol
// ======================================================================

#ifndef RADIO_FRAME_DETECTOR_HPP
#define RADIO_FRAME_DETECTOR_HPP

#include <Svc/FrameAccumulator/FrameDetector.hpp>
#include "keys_template/TransceiverConfig.hpp"
#include "LinkProtocols/RadioLinkProtocol/CRC16.hpp"
#include "Fw/Types/Assert.hpp"

namespace RadioLinkProtocol {

    class RadioFrameDetector : public Svc::FrameDetector {
        public:
            //! Constructor
            RadioFrameDetector();

            ~RadioFrameDetector();

            //! Setup the frame detector with injected radio frame configuration.
            //! \param config  Populated RadioFrameConfig from TransceiverConfig.hpp
            void setup(const TransceiverConfig::RadioFrameConfig& config);

            Status detect(const Types::CircularBuffer& ring, FwSizeType& frameSize) const override;

            //! Compute CRC-16 using the stored config's polynomial and init value.
            //! Delegates to the shared crc16() free function in CRC16.hpp.
            inline U16 calculate_crc16(const U8* data, U32 size) const {
                return RadioLinkProtocol::crc16(data, size, m_config);
            }

        private:
            TransceiverConfig::RadioFrameConfig m_config;
    };
}

#endif
