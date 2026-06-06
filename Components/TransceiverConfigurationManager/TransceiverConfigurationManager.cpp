#include "Components/TransceiverConfigurationManager/TransceiverConfigurationManager.hpp"
#include <cstring>
#include "Fw/Types/BasicTypes.hpp"

namespace Components {

  TransceiverConfigurationManager::TransceiverConfigurationManager(
      const char* const compName)
    : TransceiverConfigurationManagerComponentBase(compName)
  {}

  TransceiverConfigurationManager::~TransceiverConfigurationManager() {}

  void TransceiverConfigurationManager::configureSettings_cmdHandler(
      FwOpcodeType opCode,
      U32 cmdSeq
  ) {
    // Stub: implement here.
    // The radio configuration sequence via I2C has been removed.
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  void TransceiverConfigurationManager::pingIn_handler(FwIndexType portNum, U32 key) {
    this->pingOut_out(portNum, key);
  }

  void TransceiverConfigurationManager::configureSettings() {
    // Stub: implement here.
  }

  void TransceiverConfigurationManager::sendConfigCommand(
      const char* message,
      const char* command
  ) {
    // Stub: implement here.
  }

  Fw::Buffer TransceiverConfigurationManager::sendI2cCmd(
      const char* command, 
      U32 writeSize, 
      U32 readSize
  ) {
    // Stub: implement here.
    // I2C transaction logic has been removed.
    return Fw::Buffer();
  }

  void TransceiverConfigurationManager::checkI2cStatus(Drv::I2cStatus i2cStatus) {
    // Stub: implement here.
  }

  TransceiverConfigurationManager::Response
  TransceiverConfigurationManager::parseBuffer(Fw::Buffer buffer) {
    // Stub: implement here.
    return Response();
  }

} // namespace Components
