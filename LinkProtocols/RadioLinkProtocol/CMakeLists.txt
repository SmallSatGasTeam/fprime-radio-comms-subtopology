add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/RadioFramer/")
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/RadioDeframer/")
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/RadioProtocol/")

register_fprime_module(
  SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/RadioFrameDetector/RadioFrameDetector.cpp"
  DEPENDS
    Fw_Types
    Fw_Buffer
    Svc_FrameAccumulator
)


