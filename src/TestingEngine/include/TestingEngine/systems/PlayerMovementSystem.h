#ifndef TESTING_ENGINE_PLAYER_MOVEMENT_SYSTEM_H
#define TESTING_ENGINE_PLAYER_MOVEMENT_SYSTEM_H
#include <SM/Precision.h>
#include <SM/Orientations.h>
#include <SGE/managers/View.h>
#include <SGE/managers/CommandBuffer.h>
#include <SGE/systems/SystemDescriptor.h>
#include <SGE/physics/Directions3.h>

// Reads.
#include <SGE/core/ecs/components/CPlayerController3.h>
#include <TestingEngine/components/CCameraControl3.h>
#include <SGE/core/globalContext/contexts/CameraContext.h>

// Writes
#include <SGE/core/ecs/components/CTransform3.h>
#include <SGE/physics/forceComponents/CRigidBody3.h>


namespace PlayerMovementSystem {
    void update(sge::Registry &registry, sge::CommandBuffer &cmdBuffer, sm::real dt);

    sge::SystemDescriptor getSystemDescriptor();
}

#endif // TESTING_ENGINE_PLAYER_MOVEMENT_SYSTEM_H