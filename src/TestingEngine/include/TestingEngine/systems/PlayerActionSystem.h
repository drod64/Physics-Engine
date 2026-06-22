#ifndef SGE_PLAYER_ACTION_SYSTEM_H
#define SGE_PLAYER_ACTION_SYSTEM_H
#include <raylib.h>
#include <SM/Precision.h>
#include <SGE/managers/View.h>
#include <SGE/managers/CommandBuffer.h>
#include <SGE/systems/SystemDescriptor.h>
#include <TestingEngine/ScenePlayActions.h>

// Reads.
#include <SGE/core/ecs/components/CCamera3.h>
#include <SGE/managers/registryResources/PlayerInputResource.h>

// Writes.
#include <SGE/core/ecs/components/CTransform3.h>

namespace PlayerActionSystem {
    void update(sge::Registry &registry, sge::CommandBuffer &cmd, sm::real);

    sge::SystemDescriptor getSystemDescription();
}

#endif // SGE_PLAYER_ACTION_SYSTEM_H