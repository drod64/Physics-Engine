#include <TestingEngine/systems/PlayerActionSystem.h>

void PlayerActionSystem::update(sge::Registry &registry, sge::CommandBuffer &cmd, sm::real)
{
    const auto &playerInput = registry.getResource<sge::PlayerInputResource>();
    auto cameraView = registry.viewAll<sge::CTransform3, sge::CCamera3>();

    for (const sge::Entity &e : cameraView)
    {
        auto &t3 = cameraView.get<sge::CTransform3>(e);

        if (playerInput.isActionHeld(ScenePlayAction::MoveForward))
        {
            t3.position += {0, 0, 1};
        }
        
        if (playerInput.isActionHeld(ScenePlayAction::MoveBackward))
        {
            t3.position += {0, 0, -1};
        }
        
        if (playerInput.isActionHeld(ScenePlayAction::MoveLeft))
        {
            t3.position += {1, 0, 0};
        }
        
        if (playerInput.isActionHeld(ScenePlayAction::MoveRight))
        {
            t3.position += {-1, 0, 0};
        }
        
        if (playerInput.isActionHeld(ScenePlayAction::MoveDown))
        {
            t3.position += {0, -1, 0};
        }
        
        if (playerInput.isActionHeld(ScenePlayAction::MoveUp))
        {
            t3.position += {0, 1, 0};
        }
    }
}

sge::SystemDescriptor PlayerActionSystem::getSystemDescription()
{
    sge::SystemDescriptor desc;

    // System phase.
    desc.phase = sge::ExecutionPhase::PreUpdate;

    // System functor.
    desc.functionPtr = &PlayerActionSystem::update;
    
    // System component reads.
    desc.componentReads.set(sge::ComponentIDCounter::get<sge::CCamera3>());

    // System component writes.
    desc.componentWrites.set(sge::ComponentIDCounter::get<sge::CTransform3>());

    // System resource reads
    desc.resourceReads.set(sge::ResourceIDCounter::get<sge::PlayerInputResource>());

    // No system resource writes.
    
    // System name.
    desc.name = "PlayerActionSystem";

    return desc;
}