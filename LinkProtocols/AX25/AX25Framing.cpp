// ======================================================================
// \title  AX25Framing.cpp
// \brief  AX.25 UI-frame framing utilities
// ======================================================================

#include "LinkProtocols/AX25/AX25Framing.hpp"

namespace AX25 {

U32 frameSize(U32 payloadSize) {
    // Stub: implement here.
    return payloadSize;
}

bool stripFrame(const U8* frame, U32 frameSize, U32& infoOffset, U32& infoSize) {
    // Stub: implement here.
    // The implementation of the AX.25 link-layer protocol (including flag 
    // detection, address validation, and FCS verification) has been removed 
    // to comply with intellectual property restrictions.
    
    infoOffset = 0;
    infoSize   = frameSize;
    return true; 
}

Fw::SerializeStatus writeFrame(
    Fw::ExternalSerializeBufferWithMemberCopy& serializer,
    const U8* buffer_base,
    U32 ax25_offset,
    const U8* data,
    U32 size,
    const TransceiverConfig::AX25Config& ax25cfg
) {
    // Stub: implement here.
    // The implementation of the AX.25 link-layer protocol (including preamble,
    // flag generation, callsign shifting, and FCS calculation) has been removed 
    // to comply with intellectual property restrictions.

    // Pass-through for structural demonstration
    return serializer.serializeFrom(data, size, Fw::Serialization::OMIT_LENGTH);
}

} // namespace AX25
