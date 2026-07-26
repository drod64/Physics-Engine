#include <SGE/systems/CameraSystem.h>

void sge::CameraSystem::update(sge::Registry &registry, sge::CommandBuffer &buffer, sm::real dt)
{
    // Read CameraContext
    sge::Entity activeCamera = registry.getContext<sge::CameraContext>().activeCamera;

    if (activeCamera == sge::Entity::INVALID) return;

    if (!registry.hasComponent<sge::CTransform3>(activeCamera) ||
        !registry.hasComponent<sge::CCamera3>(activeCamera)) return;

    // Read CTransform3
    const auto &t3 = registry.getComponent<sge::CTransform3>(activeCamera);
    // Write CCamera3
    auto &camera = registry.getComponent<sge::CCamera3>(activeCamera);

    camera.forward = t3.orientation * sge::Directions3::GLOBAL_FORWARD;
    camera.right = t3.orientation * sge::Directions3::GLOBAL_RIGHT;
    camera.up = t3.orientation * sge::Directions3::GLOBAL_UP;

    sm::Vec3 target = t3.position + camera.forward;
}

sge::SystemDescriptor sge::CameraSystem::getSystemDescription()
{
    sge::SystemDescriptor desc;

    desc.phase = sge::ExecutionPhase::PostUpdate;

    desc.contexts.reads.set(sge::GlobalContextIDCounter::get<sge::CameraContext>());
    desc.components.reads.set(sge::ComponentIDCounter::get<sge::CTransform3>());
    desc.components.writes.set(sge::ComponentIDCounter::get<sge::CCamera3>());

    desc.name = "CameraSystem";

    desc.functionPtr = &sge::CameraSystem::update;

    return desc;
}