module RadioDeployment {

  # ----------------------------------------------------------------------
  # Symbolic constants for port numbers
  # ----------------------------------------------------------------------

  enum Ports_RateGroups {
    rateGroup1
    rateGroup2
    rateGroup3
  }

  topology RadioDeployment {

  # ----------------------------------------------------------------------
  # Subtopology imports
  # ----------------------------------------------------------------------
    import CdhCore.Subtopology
    import DataProducts.Subtopology
    import FileHandling.Subtopology

    # import ComCcsds.Subtopology 

    import RadioProtocol.Subtopology 
        
  # ----------------------------------------------------------------------
  # Instances used in the topology
  # ----------------------------------------------------------------------
    instance chronoTime
    instance rateGroup1
    instance rateGroup2
    instance rateGroup3
    instance rateGroupDriver
    instance systemResources
    instance timer
    instance comDriver
    instance cmdSeq

    instance i2cLinuxDriver
    instance LinuxUartDriver
    instance transceiverConfigManager
    instance transceiverCommsManager

  # ----------------------------------------------------------------------
  # Pattern graph specifiers
  # ----------------------------------------------------------------------

    command connections instance CdhCore.cmdDisp
    event connections instance CdhCore.events
    telemetry connections instance CdhCore.tlmSend
    text event connections instance CdhCore.textLogger
    health connections instance CdhCore.$health
    param connections instance FileHandling.prmDb
    time connections instance chronoTime

  # ----------------------------------------------------------------------
  # Telemetry packets (only used when TlmPacketizer is used)
  # ----------------------------------------------------------------------

    # include "TransceiverDeployment2Packets.fppi"

  # ----------------------------------------------------------------------
  # Direct graph specifiers
  # ----------------------------------------------------------------------

    connections RadioProtocol_CdhCore {
      # Core events and telemetry to communication queue
      CdhCore.events.PktSend -> RadioProtocol.comQueue.comPacketQueueIn[RadioProtocol.Ports_ComPacketQueue.EVENTS]
      CdhCore.tlmSend.PktSend -> RadioProtocol.comQueue.comPacketQueueIn[RadioProtocol.Ports_ComPacketQueue.TELEMETRY]

      # Router to Command Dispatcher
      RadioProtocol.fprimeRouter.commandOut -> CdhCore.cmdDisp.seqCmdBuff
      CdhCore.cmdDisp.seqCmdStatus -> RadioProtocol.fprimeRouter.cmdResponseIn
      
    }

    connections RadioProtocol_FileHandling {
      # File Downlink to Communication Queue
      FileHandling.fileDownlink.bufferSendOut -> RadioProtocol.comQueue.bufferQueueIn[RadioProtocol.Ports_ComBufferQueue.FILE]
      RadioProtocol.comQueue.bufferReturnOut[RadioProtocol.Ports_ComBufferQueue.FILE] -> FileHandling.fileDownlink.bufferReturn

      # Router to File Uplink
      RadioProtocol.fprimeRouter.fileOut -> FileHandling.fileUplink.bufferSendIn
      FileHandling.fileUplink.bufferSendOut -> RadioProtocol.fprimeRouter.fileBufferReturnIn
    }

    connections Communications {
      # ComDriver buffer allocations
      comDriver.allocate      -> RadioProtocol.commsBufferManager.bufferGetCallee
      comDriver.deallocate    -> RadioProtocol.commsBufferManager.bufferSendIn
      
      # ComDriver <-> ComStub (Uplink)
      comDriver.$recv                     -> RadioProtocol.comStub.drvReceiveIn
      RadioProtocol.comStub.drvReceiveReturnOut -> comDriver.recvReturnIn  
      
      # ComStub <-> ComDriver (Downlink)
      RadioProtocol.comStub.drvSendOut      -> comDriver.$send            
      comDriver.ready         -> RadioProtocol.comStub.drvConnected 
    }

    connections FileHandling_DataProducts {
      # Data Products to File Downlink
      DataProducts.dpCat.fileOut -> FileHandling.fileDownlink.SendFile
      FileHandling.fileDownlink.FileComplete -> DataProducts.dpCat.fileDone
    }

    connections RateGroups {
      # timer to drive rate group
      timer.CycleOut -> rateGroupDriver.CycleIn

      # Rate group 1
      rateGroupDriver.CycleOut[Ports_RateGroups.rateGroup1] -> rateGroup1.CycleIn
      rateGroup1.RateGroupMemberOut[0] -> CdhCore.tlmSend.Run
      rateGroup1.RateGroupMemberOut[1] -> FileHandling.fileDownlink.Run
      rateGroup1.RateGroupMemberOut[2] -> systemResources.run
      rateGroup1.RateGroupMemberOut[3] -> RadioProtocol.comQueue.run
      rateGroup1.RateGroupMemberOut[4] -> transceiverCommsManager.schedIn

      # Rate group 2
      rateGroupDriver.CycleOut[Ports_RateGroups.rateGroup2] -> rateGroup2.CycleIn
      rateGroup2.RateGroupMemberOut[0] -> cmdSeq.schedIn

      # Rate group 3
      rateGroupDriver.CycleOut[Ports_RateGroups.rateGroup3] -> rateGroup3.CycleIn
      rateGroup3.RateGroupMemberOut[0] -> CdhCore.$health.Run
      rateGroup3.RateGroupMemberOut[1] -> RadioProtocol.commsBufferManager.schedIn
      rateGroup3.RateGroupMemberOut[2] -> DataProducts.dpBufferManager.schedIn
      rateGroup3.RateGroupMemberOut[3] -> DataProducts.dpWriter.schedIn
      rateGroup3.RateGroupMemberOut[4] -> DataProducts.dpMgr.schedIn
    }

    connections CdhCore_cmdSeq {
      # Command Sequencer
      cmdSeq.comCmdOut -> CdhCore.cmdDisp.seqCmdBuff
      CdhCore.cmdDisp.seqCmdStatus -> cmdSeq.cmdResponseIn
    }

    connections TransceiverManagers {
      # TransceiverConfigurationManager -- I2C
      transceiverConfigManager.i2cWriteRead -> i2cLinuxDriver.writeRead
      transceiverConfigManager.i2cRead -> i2cLinuxDriver.read
      transceiverConfigManager.i2cWrite -> i2cLinuxDriver.write

      # TransceiverConfigurationManager -- Buffer Allocation
      transceiverConfigManager.allocate -> RadioProtocol.commsBufferManager.bufferGetCallee
      transceiverConfigManager.deallocate -> RadioProtocol.commsBufferManager.bufferSendIn

      # TransceiverCommsManager -- Downlink to comQueue
      transceiverCommsManager.sendBuffer -> RadioProtocol.comQueue.bufferQueueIn[RadioProtocol.Ports_ComBufferQueue.DOWNLINK]
      RadioProtocol.comQueue.bufferReturnOut[RadioProtocol.Ports_ComBufferQueue.DOWNLINK] -> transceiverCommsManager.bufferReturn

      # TransceiverCommsManager -- Buffer Allocation
      transceiverCommsManager.allocate -> RadioProtocol.commsBufferManager.bufferGetCallee
      transceiverCommsManager.deallocate -> RadioProtocol.commsBufferManager.bufferSendIn
    }

  }

}
