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

        // Update camera orientation with raw trigonometry.
        sm::real cosPitch = real_cos(control.pitch);
        sm::real sinPitch = real_sin(control.pitch);
        sm::real cosYaw = real_cos(control.yaw);
        sm::real sinYaw = real_sin(control.yaw);

        // Calculate clean forward vector.
        sm::Vec3 cleanForward(
            cosPitch * sinYaw,
            sinPitch,
            cosPitch * cosYaw
        );

        // Normalize clean forward.
        cleanForward.normalize();

        // Update camera orientation via lookAt().
        sm::Vec3 globalUp(0, 1, 0);
        cameraT3.orientation = sm::lookAt(sm::Vec3(0, 0, 0), cleanForward, globalUp);

        // Check if entity to follow is valid.
        if (follow.targetEntity == sge::Entity ::INVALID ||
            !registry.hasComponent<sge::CTransform3>(follow.targetEntity)) continue;
        
        // Read target's CTransform3
        const auto &targetT3 = registry.getComponent<sge::CTransform3>(follow.targetEntity);
        
        // First person tracking.
        if (registry.hasComponent<TagFPSCam>(e))
        {
            cameraT3.position = targetT3.position + follow.offset;
        }
        // Third person tracking.
        else if (registry.hasComponent<TagThirdPersonCam>(e))
        {
            sm::Vec3 targetPos = targetT3.position;
            sm::Vec3 right = targetT3.orientation * sm::Vec3(1, 0, 0);
            right.normalize();

            targetPos += (right * follow.offset.x);
            targetPos.y += follow.offset.y;
            targetPos -= (cleanForward * follow.offset.z);

            sm::real alpha = static_cast<sm::real>(1) - real_exp(-follow.smoothSpeed * dt);

            cameraT3.position = sm::MathUtil::lerp(cameraT3.position, targetPos, alpha);
        }
    }
}

sge::SystemDescriptor CameraFollowSystem::getSystemDescription()
{
    sge::SystemDescriptor desc;
    desc.phase = sge::ExecutionPhase::Gameplay;

    desc.components.reads.set(sge::ComponentIDCounter::get<CCameraFollow3>());
    desc.components.reads.set(sge::ComponentIDCounter::get<CCameraControl3>());

    desc.components.writes.set(sge::ComponentIDCounter::get<sge::CTransform3>());

    desc.name = "CameraFollowSystem";

    desc.functionPtr = &CameraFollowSystem::update;

    return desc;
}