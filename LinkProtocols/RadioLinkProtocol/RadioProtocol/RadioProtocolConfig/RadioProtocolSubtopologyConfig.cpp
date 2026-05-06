#include "RadioProtocolSubtopologyConfig.hpp"

namespace RadioProtocol {
namespace Allocation {
    // This instance can be changed to use a different allocator in the ComCcsds Subtopology
    Fw::MallocAllocator mallocatorInstance;
    Fw::MemAllocator& memAllocator = mallocatorInstance;
    TransceiverConfig::RadioFrameConfig radioConfig;
    RadioLinkProtocol::RadioFrameDetector frameDetector;
}  // namespace Allocation
}  // namespace RadioProtocol
