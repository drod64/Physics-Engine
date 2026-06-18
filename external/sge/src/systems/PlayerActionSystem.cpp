#include <SGE/systems/PlayerActionSystem.h>

void sge::PlayerActionSystem::update(Registry &registry, CommandBuffer &cmd, sm::real)
{
    auto &playerInput = registry.getResource<sge::PlayerInputResource>();
    auto cameraView = registry.viewAll<sge::CTransform3, sge::CCamera3>();

    for (Entity e : cameraView)
    {
        auto &t3 = registry.getComponent<sge::CTransform3>(e);
        auto &c3 = registry.getComponent<sge::CCamera3>(e);

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