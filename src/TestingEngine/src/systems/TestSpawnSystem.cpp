#include <TestingEngine/systems/TestSpawnSystem.h>

void TestSpawnSystem::update(sge::Registry &registry, sge::CommandBuffer &cmdBuffer, sm::real dt)
{
    const auto &playerInput = registry.getResource<sge::PlayerInputResource>();

    // Testing creation of entities and addition of components.
    if (playerInput.isActionPressed(sge::Action::PrimaryAction)) // Left mouse click
    {
        ScenePlaySpawn::spawnBungeeSpring(cmdBuffer);
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

sge::SystemDescriptor TestSpawnSystem::getSystemDescriptor()
{
    sge::SystemDescriptor desc;

    // System functor.
    desc.functionPtr = &TestSpawnSystem::update;

    // System component reads.
    desc.componentReads.set(sge::ComponentIDCounter::get<sge::CGravity3>());
    desc.componentReads.set(sge::ComponentIDCounter::get<sge::CRigidBody3>());

    // No system component writes.

    // System resource reads.
    desc.resourceReads.set(sge::ResourceIDCounter::get<sge::PlayerInputResource>());

    // No system resource writes.

    // System name.
    desc.name = "TestSpawnSystem";

    return desc;
}