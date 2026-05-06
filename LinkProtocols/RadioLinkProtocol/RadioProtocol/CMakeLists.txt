add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/RadioProtocolConfig/")

register_fprime_module(
    AUTOCODER_INPUTS
        "${CMAKE_CURRENT_LIST_DIR}/RadioProtocol.fpp"
    HEADERS
        "${CMAKE_CURRENT_LIST_DIR}/SubtopologyTopologyDefs.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/PingEntries.hpp"
    DEPENDS
        LinkProtocols_RadioLinkProtocol_RadioProtocol_RadioProtocolConfig
    INTERFACE
)