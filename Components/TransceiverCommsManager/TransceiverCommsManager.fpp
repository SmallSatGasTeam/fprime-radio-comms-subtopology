module Components {

    @ Beacon state machine states
    enum BeaconState {
        SILENT          @< No transmissions (LEOP first 30 min)
        ACTIVE_BEACON   @< Normal periodic beacon
        LOW_POWER       @< Reduced beacon rate to conserve power
        DOWNLINKING     @< Beacon paused during active data downlink
        DISABLED        @< Beacon off by command
    }

    @ Custom port: allows components to set beacon state
    port SetBeaconState(newState: BeaconState)

    @ Downlink gateway and beacon state machine. All outgoing data routes through this component.
    active component TransceiverCommsManager {

        ##############################################################################
        # Data Ports
        ##############################################################################

        @ Receives data buffers from components for downlink
        async input port dataIn: Fw.BufferSend

        @ Forwards framed data to comQueue for transmission
        output port sendBuffer: Fw.BufferSend

        @ Returned buffers from comQueue after transmission
        async input port bufferReturn: Fw.BufferSend

        ##############################################################################
        # Beacon Control Ports
        ##############################################################################

        @ Allows components to change beacon state programmatically
        async input port setBeaconStateIn: Components.SetBeaconState

        @ Rate group tick for beacon timer
        async input port schedIn: Svc.Sched

        ##############################################################################
        # Commands
        ##############################################################################

        @ Test command: send a string as data through the framing stack
        async command sendData($data: string) opcode 0x01

        @ Change the beacon state machine state
        async command setBeaconState(newState: BeaconState) opcode 0x02

        ##############################################################################
        # Events
        ##############################################################################

        event beaconStateChange(oldState: BeaconState, newState: BeaconState) \
            severity activity high \
            format "Beacon state: {} -> {}"

        event transmitDataSuccess \
            severity activity high \
            format "Data sent to framing stack"

        event transmitDataFailure \
            severity warning high \
            format "Failed to send data to framing stack"

        event MemoryAllocationFailed() \
            severity warning low \
            format "Failed to allocate memory"

        ##############################################################################
        # Telemetry
        ##############################################################################

        @ Current beacon state
        telemetry beaconState: BeaconState

        @ Total packets sent to framing stack
        telemetry packetsSent: U32

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
