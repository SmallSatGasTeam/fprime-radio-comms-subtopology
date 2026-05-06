module RadioLinkProtocol {
    @ Component for deframing radio packets
    passive component RadioDeframer {

        import Svc.Deframer

        @ An invalid frame was received (checksum mismatch)
        event InvalidChecksum \
        severity warning high \
        format "Frame dropped: The transmitted frame checksum does not match that computed by the receiver"

        ###############################################################################
        # Standard AC Ports for Events 
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Port for sending textual representation of events
        text event port logTextOut

        @ Port for sending events to downlink
        event port logOut

    }
}