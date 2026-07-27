#include <TestingEngine/systems/PlayerMovementSystem.h>

void PlayerMovementSystem::update(sge::Registry &registry, sge::CommandBuffer &, sm::real dt)
{
    // Read CameraContext
    sge::Entity activeCamera = registry.getContext<sge::CameraContext>().activeCamera;

    if (activeCamera == sge::Entity::INVALID ||
        !registry.hasComponent<CCameraControl3>(activeCamera)) return;
    
    // Read CCameraControl3
    const auto &cameraCtrl = registry.getComponent<CCameraControl3>(activeCamera);

    auto playerView = registry.viewAll<sge::CPlayerController3, sge::CTransform3, sge::CRigidBody3>();
    for (const sge::Entity &e : playerView)
    {
        // Read CPlayerController3
        const auto &playerCtrl = playerView.get<sge::CPlayerController3>(e);
        // Write CTransform3
        auto &playerT3 = playerView.get<sge::CTransform3>(e);
        // Write CRigidBody3
        auto &playerR3 = playerView.get<sge::CRigidBody3>(e);

        // 1. Update "player" orientation.
        sm::Quaternion playerYawRotation = sm::fromAxisAngle(sge::Axes::GLOBAL_UP, cameraCtrl.yaw);
        playerT3.orientation = playerYawRotation;
        playerT3.orientation.normalize();

        // 2. Update player velocity.
        // 4 mph maximum target speed
        const float maxSpeed = 150;
        const float acceleration = 50.0f;

        // Calculate target speed based on player input.
        sm::Vec3 localIntentVelocity = 
            (sge::Axes::GLOBAL_FORWARD  * playerCtrl.forwardDirection   * maxSpeed) +
            (sge::Axes::GLOBAL_UP       * playerCtrl.upDirection        * maxSpeed) +
            (sge::Axes::GLOBAL_RIGHT    * playerCtrl.rightDirection     * maxSpeed);
        
        // Retrieve current speed of player.
        sm::Vec3 localPlayerVelocity = playerT3.orientation.transformInverse(playerR3.linearVelocity);
    
        // Calculate current speed axes of player.
        sm::real curForwardSpeed =      sm::Vec3::dot(localPlayerVelocity, sge::Axes::GLOBAL_FORWARD);
        sm::real curUpSpeed =           sm::Vec3::dot(localPlayerVelocity, sge::Axes::GLOBAL_UP);
        sm::real curRightSpeed =        sm::Vec3::dot(localPlayerVelocity, sge::Axes::GLOBAL_RIGHT);

        // Calculate target speed axes of player.
        sm::real targetForwardSpeed =   sm::Vec3::dot(localIntentVelocity, sge::Axes::GLOBAL_FORWARD); 
        sm::real targetUpSpeed =        sm::Vec3::dot(localIntentVelocity, sge::Axes::GLOBAL_UP); 
        sm::real targetRightSpeed =     sm::Vec3::dot(localIntentVelocity, sge::Axes::GLOBAL_RIGHT); 

        // Accumulate local velocity gradually using dt
        // This pushes current speed toward the target speed
        sm::real maxStep = acceleration * dt;
        
        // Lambda helper function.
        auto moveTowards = [](sm::real cur, sm::real target, sm::real step) {
            if (cur < target) return std::min(target, cur + step);
            if (cur > target) return std::max(target, cur - step);

            return target;
        };

        sm::real newForwardSpeed =  moveTowards(curForwardSpeed, targetForwardSpeed, maxStep);
        sm::real newUpSpeed =       moveTowards(curUpSpeed, targetUpSpeed, maxStep);
        sm::real newRightSpeed =    moveTowards(curRightSpeed, targetRightSpeed, maxStep);

        localPlayerVelocity =
        (sge::Axes::GLOBAL_FORWARD * newForwardSpeed) +
        (sge::Axes::GLOBAL_UP * newUpSpeed) +
        (sge::Axes::GLOBAL_RIGHT * newRightSpeed);

        playerR3.linearVelocity = playerT3.orientation.transform(localPlayerVelocity);
    }
}

sge::SystemDescriptor PlayerMovementSystem::getSystemDescriptor()
{
    sge::SystemDescriptor desc;

    // System phase
    desc.phase = sge::ExecutionPhase::Gameplay;

    // System functor.
    desc.functionPtr = &PlayerMovementSystem::update;

    // System reads.
    desc.components.reads.set(sge::ComponentIDCounter::get<sge::CPlayerController3>());
    desc.components.reads.set(sge::ComponentIDCounter::get<CCameraControl3>());
    desc.contexts.reads.set(sge::GlobalContextIDCounter::get<sge::CameraContext>());

    // System writes.
    desc.components.writes.set(sge::ComponentIDCounter::get<sge::CTransform3>());
    desc.components.writes.set(sge::ComponentIDCounter::get<sge::CRigidBody3>());

    // System requires.
    desc.components.requirez.set(sge::ComponentIDCounter::get<sge::CPlayerController3>());

    // System name.
    desc.name = "PlayerMovementSystem";

    return desc;
}