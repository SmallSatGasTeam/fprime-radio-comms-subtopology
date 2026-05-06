// ======================================================================
// \title  RadioFramer.cpp
// \author xtilloo
// \brief  cpp file for RadioFramer component implementation class
// ======================================================================

#include "LinkProtocols/RadioLinkProtocol/RadioFramer/RadioFramer.hpp"

namespace RadioLinkProtocol {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

RadioFramer::RadioFramer(const char* const compName)
    : RadioFramerComponentBase(compName) {}

void RadioFramer::setup(const TransceiverConfig::Config& config) {
  m_config = config;
}

RadioFramer::~RadioFramer() {}

// ----------------------------------------------------------------------
// Handler implementations for typed input ports
// ----------------------------------------------------------------------

void RadioFramer::comStatusIn_handler(
  FwIndexType portNum,
  Fw::Success &condition
) {
  if (this->isConnected_comStatusOut_OutputPort(portNum)) {
    this->comStatusOut_out(portNum, condition);
  }
}

void RadioFramer::dataIn_handler(
  FwIndexType portNum, 
  Fw::Buffer &data,
  const ComCfg::FrameContext &context
) {
  const U32 data_size = data.getSize();
  frameData(portNum, data.getData(), data_size, context);

  // Return the input buffer to the sender
  this->dataReturnOut_out(portNum, data, context);
}

void RadioFramer::dataReturnIn_handler(
  FwIndexType portNum, 
  Fw::Buffer &data,
  const ComCfg::FrameContext &context
) {
  this->bufferDeallocate_out(0, data);
}

// -------------------------------------------------------------------------
// Helper functions
// -------------------------------------------------------------------------

void RadioFramer::frameData(FwIndexType portNum, const U8* const data, const U32 size, const ComCfg::FrameContext &context) {
    FW_ASSERT(data != nullptr);

    // Stub: pass-through. Implement your licensed framing here.
    const U32 total_len = size;

    Fw::Buffer buffer = this->bufferAllocate_out(0, total_len);
    if (buffer.getSize() < total_len) {
        if (buffer.isValid()) {
            this->bufferDeallocate_out(0, buffer);
        }
        return;
    }

    memcpy(buffer.getData(), data, size);

    buffer.setSize(total_len);
    this->dataOut_out(portNum, buffer, context);
}

} // namespace RadioLinkProtocol
