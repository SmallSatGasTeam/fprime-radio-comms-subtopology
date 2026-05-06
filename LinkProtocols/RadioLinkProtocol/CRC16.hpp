// ======================================================================
// \title  CRC16.hpp
// \brief  CRC-16 utility for the Radio link protocol stack
// ======================================================================

#ifndef RadioLinkProtocol_CRC16_HPP
#define RadioLinkProtocol_CRC16_HPP

#include <Fw/Types/BasicTypes.hpp>
#include "keys_template/TransceiverConfig.hpp"

namespace RadioLinkProtocol {

    //! Compute a CRC-16 checksum using the polynomial and initial value
    //! specified in the supplied RadioFrameConfig.
    //!
    //! Standard CRC-16-CCITT values: crcPoly=0x1021, crcInit=0xFFFF.
    //! Other variants (e.g., CRC-16/KERMIT, CRC-16/BUYPASS) can be selected
    //! by setting different crcPoly / crcInit values in RadioFrameConfig.
    //!
    //! \param data   Pointer to the data buffer
    //! \param size   Number of bytes to process
    //! \param cfg    RadioFrameConfig providing crcPoly and crcInit
    //! \return       The computed 16-bit CRC value
    U16 crc16(const U8* data, U32 size, const TransceiverConfig::RadioFrameConfig& cfg);

} // namespace RadioLinkProtocol

#endif // RadioLinkProtocol_CRC16_HPP
