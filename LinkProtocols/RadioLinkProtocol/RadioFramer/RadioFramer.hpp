// ======================================================================
// \title  RadioFramer.hpp
// \author xtilloo
// \brief  hpp file for RadioFramer component implementation class
// ======================================================================

#ifndef RadioLinkProtocol_RadioFramer_HPP
#define RadioLinkProtocol_RadioFramer_HPP

#include "LinkProtocols/RadioLinkProtocol/RadioFramer/RadioFramerComponentAc.hpp"
#include "LinkProtocols/RadioLinkProtocol/RadioFrameDetector/RadioFrameDetector.hpp"
#include "keys_template/TransceiverConfig.hpp"

namespace RadioLinkProtocol {

class RadioFramer final : public RadioFramerComponentBase {

public:
  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  //! Construct RadioFramer with injected configuration.
  //! The config is copied by value.
  //! \param compName  Component name
  RadioFramer(const char* const compName);

  //! Setup the framer's config values
  //! \param config    Fully populated TransceiverConfig::Config
  void setup(const TransceiverConfig::Config& config);

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

  TransceiverConfig::Config m_config;

};

} // namespace RadioLinkProtocol

#endif
