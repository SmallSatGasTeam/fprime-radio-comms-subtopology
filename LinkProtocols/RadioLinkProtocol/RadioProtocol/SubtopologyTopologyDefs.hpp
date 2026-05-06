#ifndef RADIOPROTOCOLSUBTOPOLOGY_DEFS_HPP
#define RADIOPROTOCOLSUBTOPOLOGY_DEFS_HPP

#include "fprime/Fw/Types/MallocAllocator.hpp"
#include "fprime/Svc/BufferManager/BufferManager.hpp"
#include "RadioProtocolConfig/RadioProtocolSubtopologyConfig.hpp"
#include "LinkProtocols/RadioLinkProtocol/RadioProtocol/RadioProtocolConfig/FppConstantsAc.hpp"
#include "LinkProtocols/RadioLinkProtocol/RadioFrameDetector/RadioFrameDetector.hpp"

namespace RadioProtocol {
struct SubtopologyState {};

struct TopologyState {
    SubtopologyState RadioProtocol;
};
}  // namespace RadioProtocol

#endif