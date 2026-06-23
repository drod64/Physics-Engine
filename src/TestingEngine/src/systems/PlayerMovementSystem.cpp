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
        if (r3.velocity.x < targetX) r3.velocity.x = std::min(targetX, r3.velocity.x + acceleration * dt);
        else if (r3.velocity.x > targetX) r3.velocity.x = std::max(targetX, r3.velocity.x - acceleration * dt);

        if (r3.velocity.z < targetZ) r3.velocity.z = std::min(targetZ, r3.velocity.z + acceleration * dt);
        else if (r3.velocity.z > targetZ) r3.velocity.z = std::max(targetZ, r3.velocity.z - acceleration * dt);
        
        if (r3.velocity.y < targetY) r3.velocity.y = std::min(targetY, r3.velocity.y + acceleration * dt);
        else if (r3.velocity.y > targetY) r3.velocity.y = std::max(targetY, r3.velocity.y - acceleration * dt);
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