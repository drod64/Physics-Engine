#ifndef TESTING_ENGINE_DISPATCHER_SYSTEM_H
#define TESTING_ENGINE_INPUT_DISPATCHER_SYSTEM_H
#include <raylib.h>
#include <SM/Precision.h>
#include <SGE/managers/View.h>
#include <SGE/managers/CommandBuffer.h>
#include <SGE/systems/SystemDescriptor.h>
#include <SGE/core/globalContext/GlobalContextIDCounter.h>
#include <TestingEngine/ScenePlayActions.h>


// Writes.
#include <SGE/core/globalContext/contexts/PlayerInputContext.h>
#include <SGE/core/ecs/components/CPlayerController3.h>

namespace InputDispatcherSystem {
    void update(sge::Registry &registry, sge::CommandBuffer &cmd, sm::real);

    sge::SystemDescriptor getSystemDescription();
}

#endif // TESTING_ENGINE_INPUT_DISPATCHER_SYSTEM_H