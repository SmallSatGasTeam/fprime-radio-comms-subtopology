module Components {

    @ Component that configures the UHF transceiver via I2C.
    active component TransceiverConfigurationManager {

        ##############################################################################
        # I2C Ports
        ##############################################################################

        @ I2C READ/WRITE. This port is used to read and write data to device.
        output port i2cWriteRead: Drv.I2cWriteRead

        @ I2C READ. This port is used to read data from device.
        output port i2cRead: Drv.I2c

        @ I2C WRITE. This port is used to write data to device.
        output port i2cWrite: Drv.I2c

        ##############################################################################
        # Commands
        ##############################################################################

        @ Configure radio settings
        async command configureSettings() opcode 0x01

        ##############################################################################
        # Events
        ##############################################################################

        event UHFSuccess \
            severity activity high \
            format "I2C transaction succeeded"

        event UHFAddressFailure \
            severity warning high \
            format "I2C invalid address"

        event UHFWriteError \
            severity warning high \
            format "I2C write failed"

        event UHFReadError \
            severity warning high \
            format "I2C read failed"

        event UHFOpenError \
            severity warning high \
            format "I2C failed to open device"

        event UHFOtherError \
            severity warning high \
            format "I2C other error"

        event MemoryAllocationFailed() \
            severity warning low \
            format "Failed to allocate memory"

        event debuggingEvent(response: string) \
            severity activity high \
            format "{}"

        ##############################################################################
        # Telemetry
        ##############################################################################

        @ Transceiver internal temperature
        telemetry temperature: U8

        @ Time since transceiver startup
        telemetry uptime: U8

        @ Whether transceiver is in low power mode
        telemetry lowPowerMode: bool

        ##############################################################################
        # Standard AC Ports
        ##############################################################################

        @ Port for requesting the current time
        time get port timeCaller

        @ Port for sending command registrations
        command reg port cmdRegOut

        @ Port for receiving commands
        command recv port cmdIn

        @ Port for sending command responses
        command resp port cmdResponseOut

        @ Port for sending textual representation of events
        text event port logTextOut

        @ Port for sending events to downlink
        event port logOut

        @ Port for sending telemetry channels to downlink
        telemetry port tlmOut

        @ Port to return the value of a parameter
        param get port prmGetOut

        @ Port to set the value of a parameter
        param set port prmSetOut

        @ Allocation port for a buffer
        output port allocate: Fw.BufferGet

        @ Deallocation port for buffers
        output port deallocate: Fw.BufferSend

        @ pingIn : receives health pings
        async input port pingIn: Svc.Ping

        @ pingOut : returns health ping
        output port pingOut: Svc.Ping
    }
}
