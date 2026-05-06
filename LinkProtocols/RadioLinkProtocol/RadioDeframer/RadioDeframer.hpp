// ======================================================================
// \title  RadioDeframer.hpp
// \author xtilloo
// \brief  hpp file for RadioDeframer component implementation class
// ======================================================================

#ifndef RadioLinkProtocol_RadioDeframer_HPP
#define RadioLinkProtocol_RadioDeframer_HPP

#include "LinkProtocols/RadioLinkProtocol/RadioDeframer/RadioDeframerComponentAc.hpp"
#include "Svc/FramingProtocol/DeframingProtocol.hpp"
#include "keys_template/TransceiverConfig.hpp"
#include "LinkProtocols/RadioLinkProtocol/CRC16.hpp"


namespace RadioLinkProtocol {

class RadioDeframer : public RadioDeframerComponentBase {

public:
  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  //! Construct RadioDeframer with injected configuration.
  //! The config is copied by value.
  //! \param compName  Component name
  //! \param config    Fully populated TransceiverConfig::Config
  RadioDeframer(const char* const compName);

  //! Setup the Deframer's configuration values
  //! \param config    Fully populated TransceiverConfig::config
  void setup(const TransceiverConfig::Config& config);

  //! Destroy RadioDeframer object
  ~RadioDeframer();

  //! Validate the buffer per the configured radio's frame check (e.g., CRC).
  //! \param dataBuffer: Buffer containing data to validate
  //! \param size: The data size in bytes
  //! \return true if the buffer is a valid frame, false otherwise
  bool validateFrame(Fw::Buffer &dataBuffer, U32 size);

private:
  // ----------------------------------------------------------------------
  // Handler implementations for typed input ports
  // ----------------------------------------------------------------------

  //! Handler implementation for dataIn
  //!
  //! Port to receive framed data, with optional context
  void dataIn_handler(FwIndexType portNum, //!< The port number
                      Fw::Buffer &data,
                      const ComCfg::FrameContext &context) override;

  //! Handler implementation for dataReturnIn
  //!
  //! Port receiving back ownership of sent buffers
  void dataReturnIn_handler(
    FwIndexType portNum, //!< The port number
    Fw::Buffer &data,
    const ComCfg::FrameContext &context) override;

  TransceiverConfig::Config m_config;

};

} // namespace RadioLinkProtocol

#endif
