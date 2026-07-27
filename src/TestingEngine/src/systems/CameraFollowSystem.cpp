#include <TestingEngine/systems/CameraFollowSystem.h>

void CameraFollowSystem::update(sge::Registry &registry, sge::CommandBuffer &cmd, sm::real dt)
{
    auto cameraView = registry.viewAll<CCameraControl3, CCameraFollow3, sge::CTransform3>();

    for (sge::Entity e : cameraView)
    {
        // Write CTransform3
        auto &cameraT3 = registry.getComponent<sge::CTransform3>(e);
        // Read CCameraFollow3
        const auto &follow = registry.getComponent<CCameraFollow3>(e);
        // Read CCameraControl3
        const auto &control = registry.getComponent<CCameraControl3>(e);

        cameraT3.orientation = sm::fromEuler(control.pitch, control.yaw, static_cast<sm::real>(0));

        // Check if entity to follow is valid.
        if (follow.targetEntity == sge::Entity ::INVALID ||
            !registry.hasComponent<sge::CTransform3>(follow.targetEntity)) continue;
        
        // Read target's CTransform3
        const auto &targetT3 = registry.getComponent<sge::CTransform3>(follow.targetEntity);
        
        // First person tracking.
        if (registry.hasComponent<TagFPSCam>(e))
        {
            sm::Vec3 rotatedOffset = targetT3.orientation * follow.offset;
            cameraT3.position = targetT3.position + rotatedOffset;
        }
        // Third person tracking.
        else if (registry.hasComponent<TagThirdPersonCam>(e))
        {
            sm::Vec3 targetPos = targetT3.position;

            sm::Vec3 camRight = cameraT3.orientation * sge::Axes::GLOBAL_RIGHT;
            sm::Vec3 camUp = cameraT3.orientation * sge::Axes::GLOBAL_UP;
            sm::Vec3 camForward = cameraT3.orientation * sge::Axes::GLOBAL_FORWARD;
            camRight.normalize();
            camUp.normalize();
            camForward.normalize();

            targetPos   +=      (camRight * follow.offset.x);
            targetPos   +=      (camUp * follow.offset.y);
            targetPos   +=      (camForward * follow.offset.z);

            sm::real alpha = static_cast<sm::real>(1) - real_exp(-follow.smoothSpeed * dt);
            cameraT3.position = sm::MathUtil::lerp(cameraT3.position, targetPos, alpha);
        }
    }
}

sge::SystemDescriptor CameraFollowSystem::getSystemDescription()
{
    sge::SystemDescriptor desc;
    desc.phase = sge::ExecutionPhase::PostGameplay;

    desc.components.reads.set(sge::ComponentIDCounter::get<CCameraFollow3>());
    desc.components.reads.set(sge::ComponentIDCounter::get<CCameraControl3>());
    desc.components.writes.set(sge::ComponentIDCounter::get<sge::CTransform3>());

    desc.name = "CameraFollowSystem";

    desc.functionPtr = &CameraFollowSystem::update;

    return desc;
}