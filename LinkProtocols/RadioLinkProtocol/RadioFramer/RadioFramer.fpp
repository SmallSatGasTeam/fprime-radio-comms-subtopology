module RadioLinkProtocol {
    @ Component storing the Radio link protocol's framing logic.
    passive component RadioFramer {

        import Svc.Framer

        @ Port to allocate a buffer for packet
        output port bufferAllocate: Fw.BufferGet

        @ Port to deallocate a buffer once packet is sent
        output port bufferDeallocate: Fw.BufferSend
        
        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
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

        @Port to set the value of a parameter
        param set port prmSetOut

    }
}