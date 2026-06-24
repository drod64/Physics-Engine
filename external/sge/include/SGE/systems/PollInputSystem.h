#ifndef SGE_POLL_INPUT_SYSTEM_H
#define SGE_POLL_INPUT_SYSTEM_H
#include <raylib.h>
#include <SGE/managers/View.h>
#include <SGE/managers/CommandBuffer.h>
#include <SGE/systems/SystemDescriptor.h>
#include <SGE/core/globalContext/GlobalContextIDCounter.h>
#include <SGE/core/globalContext/GlobalContext.h>

// Reads.
#include <SGE/core/globalContext/contexts/InputMappingContext.h>

// Writes.
#include <SGE/core/globalContext/contexts/PlayerInputContext.h>

// 1. translation, intention, and execution pipeline
// InputProvider -> PlayerActionSystem -> PlayerMovementSystem or any other system that depends on a specific action.
namespace sge {
    namespace PollInputSystem {
    
        void update(sge::Registry &registry, sge::CommandBuffer &cmdBuffer, sm::real);

        SystemDescriptor getSystemDescriptor();
};
}

#endif // SGE_POLL_INPUT_SYSTEM_H