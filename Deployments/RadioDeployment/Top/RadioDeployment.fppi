telemetry packets RadioDeploymentPackets {

  packet CDH id 1 group 1 {
    CdhCore.cmdDisp.CommandsDispatched

    FileHandling.fileUplink.FilesReceived
    FileHandling.fileUplink.PacketsReceived
    FileHandling.fileDownlink.FilesSent
    FileHandling.fileDownlink.PacketsSent
    FileHandling.fileManager.CommandsExecuted

    RadioDeployment.cmdSeq.CS_LoadCommands
    RadioDeployment.cmdSeq.CS_CancelCommands
    RadioDeployment.cmdSeq.CS_CommandsExecuted
    RadioDeployment.cmdSeq.CS_SequencesCompleted

    # Replace [Protocol].component.value
    RadioProtocol.comQueue.comQueueDepth
    RadioProtocol.comQueue.buffQueueDepth
    RadioProtocol.commsBufferManager.TotalBuffs
    RadioProtocol.commsBufferManager.CurrBuffs
    RadioProtocol.commsBufferManager.HiBuffs
    #RadioProtocol.tlmSend.SendLevel

    RadioDeployment.rateGroup1.RgMaxTime
    RadioDeployment.rateGroup2.RgMaxTime
    RadioDeployment.rateGroup3.RgMaxTime
  }

  packet CDHErrors id 2 group 1 {
    CdhCore.$health.PingLateWarnings

    FileHandling.fileUplink.Warnings
    FileHandling.fileDownlink.Warnings
    FileHandling.fileManager.Errors

    RadioDeployment.cmdSeq.CS_Errors
    RadioProtocol.commsBufferManager.NoBuffs
    RadioProtocol.commsBufferManager.EmptyBuffs

    RadioDeployment.rateGroup1.RgCycleSlips
    RadioDeployment.rateGroup2.RgCycleSlips
    RadioDeployment.rateGroup3.RgCycleSlips
  }

  packet SystemRes1 id 5 group 2 {
    RadioDeployment.systemResources.MEMORY_TOTAL
    RadioDeployment.systemResources.MEMORY_USED
    RadioDeployment.systemResources.NON_VOLATILE_TOTAL
    RadioDeployment.systemResources.NON_VOLATILE_FREE
  }

  packet SystemRes3 id 6 group 2 {
    RadioDeployment.systemResources.CPU
    RadioDeployment.systemResources.CPU_00
    RadioDeployment.systemResources.CPU_01
    RadioDeployment.systemResources.CPU_02
    RadioDeployment.systemResources.CPU_03
    RadioDeployment.systemResources.CPU_04
    RadioDeployment.systemResources.CPU_05
    RadioDeployment.systemResources.CPU_06
    RadioDeployment.systemResources.CPU_07
    RadioDeployment.systemResources.CPU_08
    RadioDeployment.systemResources.CPU_09
    RadioDeployment.systemResources.CPU_10
    RadioDeployment.systemResources.CPU_11
    RadioDeployment.systemResources.CPU_12
    RadioDeployment.systemResources.CPU_13
    RadioDeployment.systemResources.CPU_14
    RadioDeployment.systemResources.CPU_15
  }

  packet DataProducts id 21 group 3 {
    DataProducts.dpCat.CatalogDps
    DataProducts.dpCat.DpsSent

    DataProducts.dpMgr.NumSuccessfulAllocations
    DataProducts.dpMgr.NumFailedAllocations
    DataProducts.dpMgr.NumDataProducts
    DataProducts.dpMgr.NumBytes

    DataProducts.dpWriter.NumBuffersReceived
    DataProducts.dpWriter.NumBytesWritten
    DataProducts.dpWriter.NumSuccessfulWrites
    DataProducts.dpWriter.NumFailedWrites
    DataProducts.dpWriter.NumErrors

    DataProducts.dpBufferManager.TotalBuffs
    DataProducts.dpBufferManager.CurrBuffs
    DataProducts.dpBufferManager.HiBuffs
    DataProducts.dpBufferManager.NoBuffs
    DataProducts.dpBufferManager.EmptyBuffs
  }

  packet Version1 id 22 group 2 {
    CdhCore.version.FrameworkVersion
    CdhCore.version.ProjectVersion
  }

  packet Version_Library1 id 23 group 2 {
    CdhCore.version.LibraryVersion01
    CdhCore.version.LibraryVersion02
  }

  packet Version_Library2 id 24 group 2 {
    CdhCore.version.LibraryVersion03
    CdhCore.version.LibraryVersion04
  }

  packet Version_Library3 id 25 group 2 {
    CdhCore.version.LibraryVersion05
    CdhCore.version.LibraryVersion06
  }

  packet Version_Library4 id 26 group 2 {
    CdhCore.version.LibraryVersion07
    CdhCore.version.LibraryVersion08
  }

  packet Version_Library5 id 27 group 2 {
    CdhCore.version.LibraryVersion09
    CdhCore.version.LibraryVersion10
  }

  packet Version_Custom1 id 28 group 2 {
    CdhCore.version.CustomVersion01
  }

  packet Version_Custom2 id 29 group 2 {
    CdhCore.version.CustomVersion02
  }

  packet Version_Custom3 id 30 group 2 {
    CdhCore.version.CustomVersion03
  }

  packet Version_Custom4 id 31 group 2 {
    CdhCore.version.CustomVersion04
  }

  packet Version_Custom5 id 32 group 2 {
    CdhCore.version.CustomVersion05
  }

  packet Version_Custom6 id 33 group 2 {
    CdhCore.version.CustomVersion06
  }

  packet Version_Custom7 id 34 group 2 {
    CdhCore.version.CustomVersion07
  }

  packet Version_Custom8 id 35 group 2 {
    CdhCore.version.CustomVersion08
  }

  packet Version_Custom9 id 36 group 2 {
    CdhCore.version.CustomVersion09
  }

  packet Version_Custom10 id 37 group 2 {
    CdhCore.version.CustomVersion10
  }

} omit {
  CdhCore.cmdDisp.CommandErrors
}
