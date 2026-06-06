// ======================================================================
// \title  RadioDeframer.hpp
// \author xtilloo
// \brief  hpp file for RadioDeframer component implementation class
// ======================================================================

#ifndef RadioLinkProtocol_RadioDeframer_HPP
#define RadioLinkProtocol_RadioDeframer_HPP

#include "LinkProtocols/RadioLinkProtocol/RadioDeframer/RadioDeframerComponentAc.hpp"


namespace RadioLinkProtocol {

class RadioDeframer : public RadioDeframerComponentBase {

public:
  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  //! Construct RadioDeframer object
  //! \param compName  Component name
  RadioDeframer(const char* const compName);

  //! Destroy RadioDeframer object
  ~RadioDeframer();

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

};

} // namespace RadioLinkProtocol

#endif
