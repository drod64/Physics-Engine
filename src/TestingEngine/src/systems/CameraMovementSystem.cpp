#include <TestingEngine/systems/CameraMovementSystem.h>

void CameraMovementSystem::update(sge::Registry &registry, sge::CommandBuffer &cmd, sm::real dt)
{
    // Read PlayerInputContext
    const sge::PlayerInputContext &playerInput = registry.getContext<sge::PlayerInputContext>();
    const sm::real sensitivity = 0.002;
    sm::real mouseX = playerInput.getMouseDelta().x * sensitivity;
    sm::real mouseY = playerInput.getMouseDelta().y * sensitivity;
    
    auto moveCams = registry.viewAll<CCameraControl3>();
    for (sge::Entity e : moveCams)
    {
        // Write CCameraControl3
        auto &cameraCtrl = registry.getComponent<CCameraControl3>(e);
                            
        cameraCtrl.yaw += sge::Directions3::getForwardSign() * mouseX;
        cameraCtrl.pitch -= sge::Directions3::getUpSign() * mouseY;

        // Clamp yaw to be 85 degrees (1.4385 in radians) or less in both extremes.
        sm::real cap_in_radians = 1.4385;
        cameraCtrl.pitch = (cameraCtrl.pitch > cap_in_radians) ? cap_in_radians : cameraCtrl.pitch;
        cameraCtrl.pitch = (cameraCtrl.pitch < -cap_in_radians) ? -cap_in_radians : cameraCtrl.pitch;
    }
}

sge::SystemDescriptor CameraMovementSystem::getSystemDescription()
{
    sge::SystemDescriptor desc;

    desc.phase = sge::ExecutionPhase::PreUpdate;

    // Reads
    desc.contexts.reads.set(sge::GlobalContextIDCounter::get<sge::PlayerInputContext>());

    // Writes
    desc.components.writes.set(sge::ComponentIDCounter::get<CCameraControl3>());

    desc.name = "CameraMovementSystem";

    desc.functionPtr = &CameraMovementSystem::update;

    return desc;
}