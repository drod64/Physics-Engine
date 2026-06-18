#include <TestingEngine/systems/TestSpawnSystem.h>

void TestSpawnSystem::update(sge::Registry &registry, sge::CommandBuffer &cmdBuffer, sm::real dt)
{
    auto &playerInput = registry.getResource<sge::PlayerInputResource>();

    if (playerInput.isActionPressed(sge::Action::PrimaryAction))
    {
        ScenePlaySpawn::spawnProjectile(cmdBuffer, ProjectileType::PISTOL);
    }
}