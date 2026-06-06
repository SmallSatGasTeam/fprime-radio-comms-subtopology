// ======================================================================
// \title  RadioDeploymentTopologyDefs.hpp
// \brief required header file containing the required definitions for the topology autocoder
//
// ======================================================================
#ifndef RADIODEPLOYMENT_RADIODEPLOYMENTTOPOLOGYDEFS_HPP
#define RADIODEPLOYMENT_RADIODEPLOYMENTTOPOLOGYDEFS_HPP

// Subtopology PingEntries includes
#include "Svc/Subtopologies/CdhCore/PingEntries.hpp"
#include "Svc/Subtopologies/DataProducts/PingEntries.hpp"
#include "Svc/Subtopologies/FileHandling/PingEntries.hpp"

// SubtopologyTopologyDefs includes
#include "Svc/Subtopologies/CdhCore/SubtopologyTopologyDefs.hpp"
#include "Svc/Subtopologies/DataProducts/SubtopologyTopologyDefs.hpp"
#include "Svc/Subtopologies/FileHandling/SubtopologyTopologyDefs.hpp"

// ------------------------------------------------------------------------------
// Link Protocol:

// RadioProtocol Subtopology includes
#include "LinkProtocols/RadioLinkProtocol/RadioProtocol/SubtopologyTopologyDefs.hpp"
#include "LinkProtocols/RadioLinkProtocol/RadioProtocol/PingEntries.hpp"
#include "LinkProtocols/RadioLinkProtocol/RadioProtocol/Ports_ComBufferQueueEnumAc.hpp"
#include "LinkProtocols/RadioLinkProtocol/RadioProtocol/Ports_ComPacketQueueEnumAc.hpp"

// End Link Protocol
// ------------------------------------------------------------------------------

// Include autocoded FPP constants
#include "Deployments/RadioDeployment/Top/FppConstantsAc.hpp"

/**
 * \brief required ping constants
 *
 * The topology autocoder requires a WARN and FATAL constant definition for each component that supports the health-ping
 * interface. These are expressed as enum constants placed in a namespace named for the component instance. These
 * are all placed in the PingEntries namespace.
 *
 * Each constant specifies how many missed pings are allowed before a WARNING_HI/FATAL event is triggered. In the
 * following example, the health component will emit a WARNING_HI event if the component instance cmdDisp does not
 * respond for 3 pings and will FATAL if responses are not received after a total of 5 pings.
 *
 * ```c++
 * namespace PingEntries {
 * namespace cmdDisp {
 *     enum { WARN = 3, FATAL = 5 };
 * }
 * }
 * ```
 */
namespace PingEntries {
    namespace RadioDeployment_rateGroup1 {enum { WARN = 3, FATAL = 5 };}
    namespace RadioDeployment_rateGroup2 {enum { WARN = 3, FATAL = 5 };}
    namespace RadioDeployment_rateGroup3 {enum { WARN = 3, FATAL = 5 };}
    namespace RadioDeployment_cmdSeq {enum { WARN = 3, FATAL = 5 };}
    namespace RadioDeployment_transceiverConfigManager {enum { WARN = 3, FATAL = 5 };}
    namespace RadioDeployment_transceiverCommsManager {enum { WARN = 3, FATAL = 5 };}
}  // namespace PingEntries

// Definitions are placed within a namespace named after the deployment
namespace RadioDeployment {

/**
 * \brief required type definition to carry state
 *
 * The topology autocoder requires an object that carries state with the name `RadioDeployment::TopologyState`. Only the type
 * definition is required by the autocoder and the contents of this object are otherwise opaque to the autocoder. The
 * contents are entirely up to the definition of the project. This deployment uses subtopologies.
 */
struct TopologyState {
    const char* uartDevice;     //!< UART device path for communication
    U32 baudRate;               //!< Baud rate for UART communication
    CHAR* hostname;             // TCP Hostname
    U16 port;            // TCP Port num

    CdhCore::SubtopologyState cdhCore;           //!< Subtopology state for CdhCore
    RadioProtocol::SubtopologyState RadioProtocol;         //!< Subtopology state for RadioProtocol
    DataProducts::SubtopologyState dataProducts; //!< Subtopology state for DataProducts
    FileHandling::SubtopologyState fileHandling; //!< Subtopology state for FileHandling
};

namespace PingEntries = ::PingEntries;
}  // namespace RadioDeployment
#endif
