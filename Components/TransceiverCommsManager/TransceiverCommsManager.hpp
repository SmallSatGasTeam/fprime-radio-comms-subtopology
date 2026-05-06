#ifndef Components_TransceiverCommsManager_HPP
#define Components_TransceiverCommsManager_HPP

#include "Components/TransceiverCommsManager/TransceiverCommsManagerComponentAc.hpp"
#include "keys_template/TransceiverConfig.hpp"

namespace Components {

  class TransceiverCommsManager :
    public TransceiverCommsManagerComponentBase
  {
    public:
      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct TransceiverCommsManager object.
      //! \param compName  Component name
      TransceiverCommsManager(const char* const compName);

      //! Destroy TransceiverCommsManager object
      ~TransceiverCommsManager();

      //! Setup the component with configuration
      //! \param config Fully populated TransceiverConfig::Config
      void setup(const TransceiverConfig::Config& config);

    private:

      // ----------------------------------------------------------------------
      // Command handler implementations
      // ----------------------------------------------------------------------

      //! Handler implementation for command sendData
      //!
      //! Test command that allocates a buffer, serializes the provided string,
      //! and sends it to the framing stack for transmission via the antenna.
      //! Used primarily for ground testing and verification.
      void sendData_cmdHandler(FwOpcodeType opCode,            //!< The opcode
                              U32 cmdSeq,                      //!< The command sequence number
                              const Fw::CmdStringArg& data     //!< String data to transmit
      ) override;

      //! Handler implementation for command setBeaconState
      //!
      //! Changes the beacon state machine to a new state. Allows ground operators
      //! or flight logic to control beacon transmission behavior (e.g., reduce
      //! beacon rate in low power mode, disable during downlink windows).
      void setBeaconState_cmdHandler(FwOpcodeType opCode,           //!< The opcode
                                    U32 cmdSeq,                     //!< The command sequence number
                                    Components::BeaconState newState //!< The new beacon state
      ) override;

      // ----------------------------------------------------------------------
      // Port handler implementations
      // ----------------------------------------------------------------------

      //! Handler implementation for async port dataIn
      //!
      //! Receives data buffers from other components (e.g., FlightLogic) destined
      //! for downlink transmission. Forwards the buffer to the framing stack for
      //! frame encapsulation and transmission. Automatically transitions to
      //! DOWNLINKING state to pause beacons during transmission.
      void dataIn_handler(FwIndexType portNum,  //!< The port number
                         Fw::Buffer& fwBuffer   //!< The data buffer to transmit
      ) override;

      //! Handler implementation for async port bufferReturn
      //!
      //! Receives buffers returned from the framing stack after transmission
      //! completes. Deallocates the buffer and transitions back to the previous
      //! beacon state (e.g., from DOWNLINKING back to ACTIVE_BEACON) to resume
      //! periodic beacon transmission.
      void bufferReturn_handler(FwIndexType portNum,  //!< The port number
                               Fw::Buffer& fwBuffer   //!< The returned buffer
      ) override;

      //! Handler implementation for async port setBeaconStateIn
      //!
      //! Async port version of state change, allowing other components to request
      //! beacon state transitions programmatically. Same behavior as the command
      //! version but triggered by port invocation instead of ground command.
      void setBeaconStateIn_handler(FwIndexType portNum,                        //!< The port number
                                   const Components::BeaconState& newState     //!< The new beacon state
      ) override;

      //! Handler implementation for async port schedIn
      //!
      //! Receives rate group tick (typically 1 Hz). Drives the beacon countdown
      //! timers and SILENT period initialization. On each tick, checks if the
      //! current beacon interval has expired and if so, assembles and sends a
      //! beacon message to the framing stack.
      void schedIn_handler(FwIndexType portNum,  //!< The port number
                          U32 context            //!< The rate group context
      ) override;

      //! Handler implementation for async port pingIn
      //!
      //! Receives health pings from the health monitoring system and responds
      //! to demonstrate component liveness. Required for F' health pattern.
      void pingIn_handler(FwIndexType portNum,  //!< The port number
                         U32 key                 //!< Value to return to pinger
      ) override;

      // ----------------------------------------------------------------------
      // Internal method implementations
      // ----------------------------------------------------------------------

      //! Forwards a buffer to the framing stack and updates telemetry
      //!
      //! Common helper function for sending buffers (both beacon and data) to
      //! the framing stack via the sendBuffer port. Increments the packetsSent
      //! telemetry counter and logs a transmitDataSuccess event.
      //!
      //! \param buffer The buffer to forward to the framing stack
      void forwardToFramingStack(Fw::Buffer& buffer  //!< The buffer to forward
      );

      //! Transceiver configuration
      TransceiverConfig::Config m_config;

      //! Telemetry counter for total packets sent to the framing stack
      U32 m_packetsSent = 0;
  };

} // namespace Components

#endif
