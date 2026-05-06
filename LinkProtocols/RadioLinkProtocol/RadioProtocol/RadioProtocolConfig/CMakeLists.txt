register_fprime_config(
    EXCLUDE_FROM_ALL
    SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/RadioProtocolSubtopologyConfig.cpp"
    HEADERS
        "${CMAKE_CURRENT_LIST_DIR}/RadioProtocolSubtopologyConfig.hpp"
    AUTOCODER_INPUTS
        "${CMAKE_CURRENT_LIST_DIR}/RadioProtocolConfig.fpp"
    CONFIGURATION_OVERRIDES
        "${CMAKE_CURRENT_LIST_DIR}/AcConstants.fpp"
    DEPENDS
        Fw_Types
)

