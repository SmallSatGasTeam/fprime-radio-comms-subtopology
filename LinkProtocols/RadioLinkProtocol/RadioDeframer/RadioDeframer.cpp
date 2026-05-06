// ======================================================================
// \title  RadioDeframer.cpp
// \author xtilloo
// \brief  cpp file for RadioDeframer component implementation class
// ======================================================================
#include "LinkProtocols/RadioLinkProtocol/RadioDeframer/RadioDeframer.hpp"

namespace RadioLinkProtocol {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

RadioDeframer ::RadioDeframer(const char* const compName)
    : RadioDeframerComponentBase(compName) {}

void RadioDeframer::setup(const TransceiverConfig::Config& config) {
    m_config = config;
}

RadioDeframer ::~RadioDeframer() {}

// ----------------------------------------------------------------------
// Handler implementations for typed input ports
// ----------------------------------------------------------------------

void RadioDeframer ::dataIn_handler(
    FwIndexType portNum, 
    Fw::Buffer &data,
    const ComCfg::FrameContext &context
) {
    // Stub: pass-through. Implement your licensed deframing here.
    this->dataOut_out(portNum, data, context);
}

void RadioDeframer ::dataReturnIn_handler(
    FwIndexType portNum, 
    Fw::Buffer &data,
    const ComCfg::FrameContext &context
) {
    this->dataReturnOut_out(portNum, data, context);
}

// ---------------------------------------
// Helper Functions
// ---------------------------------------

bool RadioDeframer::validateFrame(Fw::Buffer &dataBuffer, U32 size) {
    // Stub: implement your licensed validation here.
    return true;
}

} // namespace RadioLinkProtocol
