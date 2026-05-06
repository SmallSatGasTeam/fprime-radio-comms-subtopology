#ifndef RADIOPROTOCOLCONFIG_CONFIG_HPP
#define RADIOPROTOCOLCONFIG_CONFIG_HPP

#include "Fw/Types/MallocAllocator.hpp"
#include "LinkProtocols/RadioLinkProtocol/RadioFrameDetector/RadioFrameDetector.hpp"
#include "keys_template/TransceiverConfig.hpp"

namespace RadioProtocol {
namespace Allocation {
extern Fw::MemAllocator& memAllocator;
}
}  // namespace RadioProtocol

#endif
