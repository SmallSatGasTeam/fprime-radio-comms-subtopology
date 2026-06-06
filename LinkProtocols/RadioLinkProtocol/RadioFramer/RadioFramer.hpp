// ======================================================================
// \title  RadioFramer.hpp
// \author xtilloo
// \brief  hpp file for RadioFramer component implementation class
// ======================================================================

#ifndef RadioLinkProtocol_RadioFramer_HPP
#define RadioLinkProtocol_RadioFramer_HPP

#include "LinkProtocols/RadioLinkProtocol/RadioFramer/RadioFramerComponentAc.hpp"

namespace RadioLinkProtocol {

class RadioFramer final : public RadioFramerComponentBase {

public:
  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  //! Construct RadioFramer object
  //! \param compName  Component name
  RadioFramer(const char* const compName);

  //! Destroy RadioFramer object
  ~RadioFramer();

private:
  // ----------------------------------------------------------------------
  // Handler implementations for typed input ports
  // ----------------------------------------------------------------------

  //! Handler implementation for comStatusIn
  //!
  //! Port receiving the general status from the downstream component
  //! indicating it is ready or not-ready for more input
  void comStatusIn_handler(FwIndexType portNum,   //!< The port number
                           Fw::Success &condition //!< Condition success/failure
                           ) override;

  //! Handler implementation for dataIn
  //!
  //! Port to receive data to frame, in a Fw::Buffer with optional context
  void dataIn_handler(FwIndexType portNum, //!< The port number
                      Fw::Buffer &data,
                      const ComCfg::FrameContext &context) override;

  //! Handler implementation for dataReturnIn
  //!
  //! Buffer coming from a deallocate call in a ComDriver component
  void dataReturnIn_handler(FwIndexType portNum, //!< The port number
                            Fw::Buffer &data,
                            const ComCfg::FrameContext &context) override;

  void frameData(FwIndexType portNum, const U8* const data, const U32 size, const ComCfg::FrameContext &context);

};

} // namespace RadioLinkProtocol

#endif
