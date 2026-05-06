// ======================================================================
// \title  AX25Framing.hpp
// \brief  AX.25 UI-frame framing utilities (STUBBED for IP compliance)
// ======================================================================

#ifndef AX25_AX25_FRAMING_HPP
#define AX25_AX25_FRAMING_HPP

#include <Fw/Buffer/Buffer.hpp>
#include <Fw/Types/BasicTypes.hpp>
#include <Fw/Types/Serializable.hpp>
#include "keys_template/TransceiverConfig.hpp"

namespace AX25 {

    //! Returns the total AX.25 frame size for a given payload size
    U32 frameSize(U32 payloadSize);

    //! Checks whether a buffer starts with a valid AX.25 UI frame and returns the
    //! location and size of the embedded info (payload) field.
    //!
    //! \param frame      Pointer to the first byte of the AX.25 frame
    //! \param frameSize  Total number of bytes in the AX.25 frame
    //! \param infoOffset [out] Byte offset from frame to the start of the info field
    //! \param infoSize   [out] Size of the info field in bytes
    //! \return           true if a valid AX.25 wrapper was detected, false otherwise
    bool stripFrame(
        const U8* frame,
        U32 frameSize,
        U32& infoOffset,
        U32& infoSize
    );

    //! Writes an AX.25 UI frame into the serializer.
    //!
    //! \param serializer   The serializer to write into (must have sufficient capacity)
    //! \param buffer_base  Pointer to the start of the output buffer (used for in-place FCS computation)
    //! \param ax25_offset  Byte offset in buffer_base where this AX.25 frame starts
    //! \param data         Payload data pointer
    //! \param size         Payload size in bytes
    //! \param ax25cfg      AX.25 callsign and SSID configuration
    //! \return             Fw::FW_SERIALIZE_OK on success
    Fw::SerializeStatus writeFrame(
        Fw::ExternalSerializeBufferWithMemberCopy& serializer,
        const U8* buffer_base,
        U32 ax25_offset,
        const U8* data,
        U32 size,
        const TransceiverConfig::AX25Config& ax25cfg
    );

} // namespace AX25

#endif
