#include <TestingEngine/systems/TestSpawnSystem.h>

void TestSpawnSystem::update(sge::Registry &registry, sge::CommandBuffer &cmdBuffer, sm::real dt)
{
    auto &playerInput = registry.getResource<sge::PlayerInputResource>();

    // Testing creation of entities and addition of components.
    if (playerInput.isActionPressed(sge::Action::PrimaryAction)) // Left mouse click
    {
        ScenePlaySpawn::spawnProjectile(cmdBuffer, ProjectileType::PISTOL);
    }


    // Testing removal of components.
    if (playerInput.isActionPressed(sge::Action::SecondaryAction)) // Right mouse click
    {
        auto gravity3View = registry.viewAll<sge::CGravity3>();

        for (sge::Entity e : gravity3View)
        {
            cmdBuffer.removeComponentDeferred<sge::CGravity3>(e);
        }
    }

    // Testing destruction of entities.
    if (playerInput.isActionPressed(sge::Action::Cancel)) // Backspace key
    {
        auto t3View = registry.viewAll<sge::CRigidBody3>();

        for (sge::Entity e : t3View)
        {
            cmdBuffer.destroyEntityDeferred(e);
        }
    }
}