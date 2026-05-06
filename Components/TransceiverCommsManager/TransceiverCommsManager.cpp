#include "Components/TransceiverCommsManager/TransceiverCommsManager.hpp"
#include <cstring>
#include "Fw/Types/BasicTypes.hpp"

namespace Components {

  TransceiverCommsManager::TransceiverCommsManager(const char* const compName)
    : TransceiverCommsManagerComponentBase(compName)
  {}

  TransceiverCommsManager::~TransceiverCommsManager() {}

  void TransceiverCommsManager::setup(const TransceiverConfig::Config& config) {
    m_config = config;
  }

  // Command Handlers

  void TransceiverCommsManager::sendData_cmdHandler(
      FwOpcodeType opCode,
      U32 cmdSeq,
      const Fw::CmdStringArg& data
  ) {
    // Stub: implement here.
    // The implementation for serializing and sending test data has been removed.
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  void TransceiverCommsManager::setBeaconState_cmdHandler(
      FwOpcodeType opCode,
      U32 cmdSeq,
      Components::BeaconState newState
  ) {
    // Stub: implement here.
    // Beacon state machine transitions have been removed.
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  // Port Handlers

  void TransceiverCommsManager::dataIn_handler(
      FwIndexType portNum,
      Fw::Buffer& fwBuffer
  ) {
    // Stub: implement here.
    // Data routing logic has been removed.
    // In a structural reference, we simply return the buffer to prevent leaks.
    this->bufferReturn_handler(portNum, fwBuffer);
  }

  void TransceiverCommsManager::bufferReturn_handler(
      FwIndexType portNum,
      Fw::Buffer& fwBuffer
  ) {
    // Structural: Return the buffer to the allocator
    if (fwBuffer.isValid()) {
      this->deallocate_out(0, fwBuffer);
    }
  }

  void TransceiverCommsManager::setBeaconStateIn_handler(
      FwIndexType portNum,
      const Components::BeaconState& newState
  ) {
    // Stub: implement here.
  }

  void TransceiverCommsManager::schedIn_handler(
      FwIndexType portNum,
      U32 context
  ) {
    // Stub: implement here.
    // Periodic beacon scheduling logic has been removed.
  }

  void TransceiverCommsManager::pingIn_handler(FwIndexType portNum, U32 key) {
    this->pingOut_out(portNum, key);
  }

  // Internal Methods

  void TransceiverCommsManager::forwardToFramingStack(Fw::Buffer& buffer) {
    // Stub: implement here.
    // The implementation for forwarding data to the framing stack has been removed.
  }

} // namespace Components
