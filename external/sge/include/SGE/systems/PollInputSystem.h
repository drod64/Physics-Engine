#ifndef SGE_POLL_INPUT_SYSTEM_H
#define SGE_POLL_INPUT_SYSTEM_H
#include <raylib.h>
#include <SGE/managers/View.h>
#include <SGE/managers/CommandBuffer.h>
#include <SGE/systems/SystemDescriptor.h>

// Reads.
#include <SGE/managers/registryResources/InputMappingResource.h>

// Writes.
#include <SGE/managers/registryResources/PlayerInputResource.h>

// 1. translation, intention, and execution pipeline
// InputProvider -> PlayerActionSystem -> PlayerMovementSystem or any other system that depends on a specific action.
namespace sge {
    namespace PollInputSystem {
    
        void update(sge::Registry &registry, sge::CommandBuffer &cmdBuffer, sm::real);

        SystemDescriptor getSystemDescriptor();
};
}

#endif // SGE_POLL_INPUT_SYSTEM_H