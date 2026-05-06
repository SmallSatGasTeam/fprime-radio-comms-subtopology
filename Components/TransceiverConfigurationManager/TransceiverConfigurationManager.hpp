#ifndef Components_TransceiverConfigurationManager_HPP
#define Components_TransceiverConfigurationManager_HPP

#include "Components/TransceiverConfigurationManager/TransceiverConfigurationManagerComponentAc.hpp"
#include "keys_template/TransceiverConfig.hpp"

namespace Components {

  class TransceiverConfigurationManager :
    public TransceiverConfigurationManagerComponentBase
  {
    public:
      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct TransceiverConfigurationManager object
      TransceiverConfigurationManager(const char* const compName  //!< The component name
      );

      //! Destroy TransceiverConfigurationManager object
      ~TransceiverConfigurationManager();

      //! Setup the component with configuration
      void setup(const TransceiverConfig::Config& config);

    private:

      // ----------------------------------------------------------------------
      // Command handler implementations
      // ----------------------------------------------------------------------

      //! Handler implementation for command configureSettings
      //!
      //! Executes the complete radio configuration sequence via I2C and enables
      //! pipe mode to allow transparent UART-to-radio bridging. Reads configuration
      //! parameters and sends configuration commands to the transceiver.
      void configureSettings_cmdHandler(FwOpcodeType opCode,  //!< The opcode
                                       U32 cmdSeq             //!< The command sequence number
      ) override;

      // ----------------------------------------------------------------------
      // Port handler implementations
      // ----------------------------------------------------------------------

      //! Handler implementation for sync port getPipeMode
      //!
      //! Returns the current pipe mode state (enabled/disabled). This getter
      //! allows other components to query whether the radio is in transparent
      //! UART-to-radio bridging mode.
      //!
      //! \return true if pipe mode is enabled, false if disabled
      bool getPipeMode_handler(FwIndexType portNum  //!< The port number
      ) override;

      //! Handler implementation for sync port setPipeMode
      //!
      //! Enables or disables pipe mode via I2C command. When enabled, the radio
      //! operates in transparent mode where UART data is transmitted over the air
      //! and data received is placed on UART.
      void setPipeMode_handler(FwIndexType portNum,  //!< The port number
                              bool enable             //!< true to enable, false to disable
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

      //! Executes the complete radio configuration sequence
      //!
      //! Performs the full I2C initialization of the Radio transceiver
      //! including frequency setting, beacon transmission period configuration,
      //! power mode configuration, and pipe mode enablement with timeout.
      //! Sends a series of configuration commands via I2C.
      void configureSettings();

      //! Sends an I2C command to the transceiver and receives response
      //!
      //! Allocates a buffer, serializes the command bytes, and executes an I2C
      //! write-then-read transaction. Returns the buffer containing the response
      //! data that can be parsed to verify success/failure.
      //!
      //! \param command      The configuration command string to send
      //! \param writeSize    Number of bytes to write
      //! \param readSize     Number of bytes to read in response
      //! \return Buffer containing the I2C response data
      Fw::Buffer sendI2cCmd(const char* command,  //!< The configuration command string
                           U32 writeSize,         //!< Bytes to write
                           U32 readSize           //!< Bytes to read
      );

      //! Validates I2C transaction status and logs appropriate events
      //!
      //! Examines the I2C status code and emits corresponding events:
      //! UHFSuccess for OK status, UHFAddressFailure, UHFWriteError, UHFReadError,
      //! UHFOpenError, or UHFOtherError for various failure conditions.
      void checkI2cStatus(Drv::I2cStatus status  //!< The I2C status to check
      );

      //! Sends a single configuration command and validates response
      //!
      //! Wrapper that sends a command via I2C, parses the response buffer,
      //! checks the status, and logs a descriptive message on completion.
      //!
      //! \param message      Human-readable description of the command for logging
      //! \param command      The configuration command string to send
      void sendConfigCommand(const char* message,  //!< Description for logging
                            const char* command    //!< The configuration command string
      );

      // Response Structure
      //! Structure to hold parsed I2C response data
      struct Response {
          Fw::String fullResponse;  //!< Complete response string from transceiver
          bool status;              //!< Whether response indicates success
          U32 size;                 //!< Size of response data
      };

      //! Parses an I2C response buffer to extract status and data
      //!
      //! Examines the response string for success indicators (e.g., "OK" prefix)
      //! and extracts relevant fields. Returns a Response structure with parsed
      //! results and status flag.
      //!
      //! \param readBuffer  Buffer containing raw I2C response data
      //! \return Response structure with parsed data and status
      Response parseBuffer(Fw::Buffer readBuffer  //!< The response buffer to parse
      );

      // Configuration
      //! Transceiver configuration (I2C address and configuration command strings)
      TransceiverConfig::Config m_config;

      // Internal State
      //! Current pipe mode state
      //!
      //! Tracks whether transparent UART-to-radio bridging is enabled.
      //! true = pipe mode active (UART operates as direct radio link)
      //! false = normal mode (explicit commands required per transmission)
      bool m_pipeModeEnabled = false;
  };

} // namespace Components

#endif
