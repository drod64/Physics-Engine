#include <TestingEngine/systems/PlayerMovementSystem.h>

void PlayerMovementSystem::update(sge::Registry &registry, sge::CommandBuffer &, sm::real dt)
{
    auto playerView = registry.viewAll<sge::CPlayerController3, sge::CRigidBody3>();

    for (const sge::Entity &e : playerView)
    {
        const auto &controller = playerView.get<sge::CPlayerController3>(e);
        auto &r3 = playerView.get<sge::CRigidBody3>(e);

        // 4 mph maximum target speed
        const float maxSpeed = 225.f; 
        const float acceleration = 50.0f; 

        // 1. Calculate target velocity based on input
        float targetX = controller.movementAxisX * maxSpeed;
        float targetZ = controller.movementAxisZ * maxSpeed;
        float targetY = controller.movementAxisY * maxSpeed;

        // 2. Accumulate velocity gradually using dt
        // This pushes current velocity toward the target velocity
        if (r3.linearVelocity.x < targetX) r3.linearVelocity.x = std::min(targetX, r3.linearVelocity.x + acceleration * dt);
        else if (r3.linearVelocity.x > targetX) r3.linearVelocity.x = std::max(targetX, r3.linearVelocity.x - acceleration * dt);

        if (r3.linearVelocity.z < targetZ) r3.linearVelocity.z = std::min(targetZ, r3.linearVelocity.z + acceleration * dt);
        else if (r3.linearVelocity.z > targetZ) r3.linearVelocity.z = std::max(targetZ, r3.linearVelocity.z - acceleration * dt);
        
        if (r3.linearVelocity.y < targetY) r3.linearVelocity.y = std::min(targetY, r3.linearVelocity.y + acceleration * dt);
        else if (r3.linearVelocity.y > targetY) r3.linearVelocity.y = std::max(targetY, r3.linearVelocity.y - acceleration * dt);
    }
}

sge::SystemDescriptor PlayerMovementSystem::getSystemDescriptor()
{
    sge::SystemDescriptor desc;

    // System phase
    desc.phase = sge::ExecutionPhase::Gameplay;

    // System functor.
    desc.functionPtr = &PlayerMovementSystem::update;

    // System component reads.
    desc.componentReads.set(sge::ComponentIDCounter::get<sge::CPlayerController3>());

    // No system component writes.

    // System component accumulations.
    desc.componentAccumulates.set(sge::ComponentIDCounter::get<sge::CRigidBody3>());

    // No system resources used.

    // System name.
    desc.name = "PlayerMovementSystem";

    return desc;
}