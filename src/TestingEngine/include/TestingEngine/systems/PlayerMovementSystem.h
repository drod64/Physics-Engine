#ifndef TESTING_ENGINE_PLAYER_MOVEMENT_SYSTEM_H
#define TESTING_ENGINE_PLAYER_MOVEMENT_SYSTEM_H
#include <SM/Precision.h>
#include <SGE/managers/View.h>
#include <SGE/managers/CommandBuffer.h>
#include <SGE/systems/SystemDescriptor.h>

// Reads.
#include <SGE/core/ecs/components/CPlayerController3.h>

// Accumulation.
#include <SGE/physics/forceComponents/CRigidBody3.h>

namespace PlayerMovementSystem {
    void update(sge::Registry &registry, sge::CommandBuffer &cmdBuffer, sm::real dt);

    sge::SystemDescriptor getSystemDescriptor();
}

#endif // TESTING_ENGINE_PLAYER_MOVEMENT_SYSTEM_H