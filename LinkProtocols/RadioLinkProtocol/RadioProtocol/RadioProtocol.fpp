module RadioProtocol {
    # ComPacket Queue enum for queue types
    enum Ports_ComPacketQueue {
        EVENTS,
        TELEMETRY 
    }

    enum Ports_ComBufferQueue {
        FILE,
        DOWNLINK
    }

    # ----------------------------------------------------------------------
    # Instances
    # ----------------------------------------------------------------------

    instance comQueue: Svc.ComQueue base id 0x10006000 \
        queue size RadioProtocolConfig.QueueSizes.comQueue \
        stack size RadioProtocolConfig.StackSizes.comQueue \
        priority RadioProtocolConfig.Priorities.comQueue \
    {
        phase Fpp.ToCpp.Phases.configComponents """
        using namespace RadioProtocol;
        Svc::ComQueue::QueueConfigurationTable configurationTable;

        configurationTable.entries[0].depth = RadioProtocolConfig::QueueDepths::events;
        configurationTable.entries[0].priority = RadioProtocolConfig::QueuePriorities::events;
        configurationTable.entries[1].depth = RadioProtocolConfig::QueueDepths::tlm;
        configurationTable.entries[1].priority = RadioProtocolConfig::QueuePriorities::tlm;
        configurationTable.entries[2].depth = RadioProtocolConfig::QueueDepths::file;
        configurationTable.entries[2].priority = RadioProtocolConfig::QueuePriorities::file;
        configurationTable.entries[3].depth = RadioProtocolConfig::QueueDepths::downlink;
        configurationTable.entries[3].priority = RadioProtocolConfig::QueuePriorities::downlink;

        RadioProtocol::comQueue.configure(configurationTable, 0, RadioProtocol::Allocation::memAllocator);
        """
        phase Fpp.ToCpp.Phases.tearDownComponents """
        RadioProtocol::comQueue.cleanup();
        """
    }

    # FrameAccumulator: Accumulates Radio frames
    instance FrameAccumulator: Svc.FrameAccumulator base id 0x10007000 \ 
    {
        phase Fpp.ToCpp.Phases.configObjects """
        // TRANSCEIVER_CONFIG must be defined in your private deployment's
        // TransceiverConfigInstance.cpp (see keys_template/README_IP_PROTECTION.md).
        extern const TransceiverConfig::Config TRANSCEIVER_CONFIG;
        RadioLinkProtocol::RadioFrameDetector frameDetector;
        """
        phase Fpp.ToCpp.Phases.configComponents """
        extern const TransceiverConfig::Config TRANSCEIVER_CONFIG;
        ConfigObjects::RadioProtocol_FrameAccumulator::frameDetector.setup(TRANSCEIVER_CONFIG.radio);
        RadioProtocol::FrameAccumulator.configure(
            ConfigObjects::RadioProtocol_FrameAccumulator::frameDetector,
            1,
            RadioProtocol::Allocation::memAllocator,
            RadioProtocolConfig::BuffMgr::frameAccumulatorSize
        );
        """
        phase Fpp.ToCpp.Phases.tearDownComponents """
        RadioProtocol::FrameAccumulator.cleanup();
        """
    }

    instance commsBufferManager: Svc.BufferManager base id 0x10008000 \
    {
        phase Fpp.ToCpp.Phases.configObjects """
        Svc::BufferManager::BufferBins bins;
        """
        phase Fpp.ToCpp.Phases.configComponents """
        memset(&ConfigObjects::RadioProtocol_commsBufferManager::bins, 0, sizeof(ConfigObjects::RadioProtocol_commsBufferManager::bins));
        ConfigObjects::RadioProtocol_commsBufferManager::bins.bins[0].bufferSize = RadioProtocolConfig::BuffMgr::commsBuffSize;
        ConfigObjects::RadioProtocol_commsBufferManager::bins.bins[0].numBuffers = RadioProtocolConfig::BuffMgr::commsBuffCount;
        RadioProtocol::commsBufferManager.setup(
            RadioProtocolConfig::BuffMgr::commsBuffMgrId,
            0,
            RadioProtocol::Allocation::memAllocator,
            ConfigObjects::RadioProtocol_commsBufferManager::bins
        );
        """
    }

    instance fprimeRouter: Svc.FprimeRouter base id 0x10009000

    instance RadioFramer: RadioLinkProtocol.RadioFramer base id 0x4100 \
    {
        phase Fpp.ToCpp.Phases.configComponents """
        extern const TransceiverConfig::Config TRANSCEIVER_CONFIG;
        RadioProtocol::RadioFramer.setup(TRANSCEIVER_CONFIG);
        """
    }

    instance RadioDeframer: RadioLinkProtocol.RadioDeframer base id 0x4900 \
    {
        phase Fpp.ToCpp.Phases.configComponents """
        extern const TransceiverConfig::Config TRANSCEIVER_CONFIG;
        RadioProtocol::RadioDeframer.setup(TRANSCEIVER_CONFIG);
        """
    }    

    instance comStub: Svc.ComStub base id 0x1000E000

    # ----------------------------------------------------------------------
    # Subtopology Definitions
    # ----------------------------------------------------------------------

    topology FramingSubtopology {
        # Active
        instance comQueue

        # Passive
        instance commsBufferManager
        instance FrameAccumulator
        instance fprimeRouter
        instance RadioFramer
        instance RadioDeframer
        instance comStub

        connections Downlink {
            # ComQueue <-> RadioFramer
            comQueue.dataOut                    ->      RadioFramer.dataIn
            RadioFramer.dataReturnOut       ->      comQueue.dataReturnIn

            # RadioFramer Buffer Management
            RadioFramer.bufferAllocate      ->      commsBufferManager.bufferGetCallee
            RadioFramer.bufferDeallocate    ->      commsBufferManager.bufferSendIn

            # comStub.comStatusOut -> RadioFramer.comStatusIn :
            #   When comStub sends our data over UART, it returns the status of 
            #   that transmission back to our framer
            comStub.comStatusOut    ->  RadioFramer.comStatusIn

            # (Outgoing) Framer -> Radio

            # RadioFramer.dataOut ->  comStub.dataIn
            #   When data is framed and we want to send it out over the antenna
            #   we call the comStub.dataIn port which handles the process of 
            #   sending data over UART.
            RadioFramer.dataOut   ->  comStub.dataIn

            # comStub.dataReturnOut -> endursatFramer.dataReturnIn :
            #   We send data to comStub in buffers, so this port returns ownership
            #   of the buffer that we sent in RadioProtoco.RadioFramer.dataOut
            comStub.dataReturnOut   ->  RadioFramer.dataReturnIn
        }

        connections Uplink {
            # Radio <-> FrameAccumulator (Done in Subtopology below)
            
            # FrameAccumulator buffer allocation
            FrameAccumulator.bufferAllocate     ->      commsBufferManager.bufferGetCallee
            FrameAccumulator.bufferDeallocate   ->      commsBufferManager.bufferSendIn

            # FrameAccumulator -> Deframer (Sends verified full frames)
            FrameAccumulator.dataOut            ->      RadioDeframer.dataIn
            RadioDeframer.dataReturnOut     ->      FrameAccumulator.dataReturnIn

            # Deframer <-> Router (Sends stripped F' packets)
            RadioDeframer.dataOut           ->      fprimeRouter.dataIn
            fprimeRouter.dataReturnOut          ->      RadioDeframer.dataReturnIn

            # comStub.dataOut -> FrameAccumulator.dataIn
            #   Data coming from the communications stack (UART) will be 
            #   piped to our communications protocol's frame accumulator
            comStub.dataOut     ->  FrameAccumulator.dataIn 

            # FrameAccumulator.dataReturnOut -> comStub.dataReturnIn
            #   Data is sent from the antenna in buffers, so when we are done processing 
            #   a buffer, we return it to where it came from (comStub)
            FrameAccumulator.dataReturnOut    ->  comStub.dataReturnIn
        }
    }

    topology Subtopology {
        import FramingSubtopology
    }
}