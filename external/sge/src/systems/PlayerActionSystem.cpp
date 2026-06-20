#include <SGE/systems/PlayerActionSystem.h>

void sge::PlayerActionSystem::update(Registry &registry, CommandBuffer &cmd, sm::real)
{
    const auto &playerInput = registry.getResource<sge::PlayerInputResource>();
    auto cameraView = registry.viewAll<sge::CTransform3, sge::CCamera3>();

    for (Entity e : cameraView)
    {
        auto &t3 = cameraView.get<sge::CTransform3>(e);

        if (playerInput.isActionHeld(sge::Action::MoveForward))
        {
            t3.position += {0, 0, 1};
        }

        if (playerInput.isActionHeld(sge::Action::MoveBackward))
        {
            t3.position += {0, 0, -1};
        }

        if (playerInput.isActionHeld(sge::Action::MoveLeft))
        {
            t3.position += {1, 0, 0};
        }

        if (playerInput.isActionHeld(sge::Action::MoveRight))
        {
            t3.position += {-1, 0, 0};
        }

        if (playerInput.isActionHeld(sge::Action::Crouch))
        {
            t3.position += {0, -1, 0};
        }

        if (playerInput.isActionHeld(sge::Action::Jump))
        {
            t3.position += {0, 1, 0};
        }
    }
}

sge::SystemDescriptor sge::PlayerActionSystem::getSystemDescription()
{
    SystemDescriptor desc;

    // System functor.
    desc.functionPtr = &sge::PlayerActionSystem::update;
    
    // System component reads.
    desc.componentReads.set(sge::ComponentIDCounter::get<sge::CTransform3>());
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