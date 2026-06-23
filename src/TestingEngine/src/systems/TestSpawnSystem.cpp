#include <TestingEngine/systems/TestSpawnSystem.h>

void TestSpawnSystem::update(sge::Registry &registry, sge::CommandBuffer &cmdBuffer, sm::real dt)
{
    auto &playerInput = registry.getOrCreateResource<sge::PlayerInputResource>();
    
    // Testing creation of entities and addition of components.
    if (playerInput.consumeActionPressed(ScenePlayAction::SpawnAnchorBungee)) // Z key
    {
        ScenePlaySpawn::spawnAnchorBungee(cmdBuffer, {0,10.5,0});
    }

    if (playerInput.consumeActionPressed(ScenePlayAction::SpawnAnchorSpring))
    {
        ScenePlaySpawn::spawnAnchorSpring(cmdBuffer, {10, 10, 0});
    }

    // if (playerInput.isActionPressed(ScenePlayAction::SpawnBungeeSpring))
    // {
    //     ScenePlaySpawn::spawnBungeeSpring(cmdBuffer);
    // }

    if (playerInput.consumeActionPressed(ScenePlayAction::SpawnBuoyant))
    {
        ScenePlaySpawn::spawnBuoyancySpring(cmdBuffer);
    }

    if (playerInput.consumeActionPressed(ScenePlayAction::SpawnFakeStiffSpring))
    {
        ScenePlaySpawn::spawnFakeSpring(cmdBuffer);
    }

    // if (playerInput.isActionPressed(ScenePlayAction::SpawnSpring))
    // {
    //     ScenePlaySpawn::spawnSpringConnection(cmdBuffer);
    // }

    // Testing removal of components.
    if (playerInput.consumeActionPressed(ScenePlayAction::RemoveGravity)) // Left mouse click
    {
        auto gravity3View = registry.viewAll<sge::CGravity3>();
        
        for (sge::Entity e : gravity3View)
        {
            cmdBuffer.removeComponentDeferred<sge::CGravity3>(e);
        }
    }
    
    // Testing destruction of entities.
    if (playerInput.consumeActionPressed(ScenePlayAction::DeleteEntities)) // Backspace key
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
    desc.phase = sge::ExecutionPhase::PreUpdate;

    // System functor.
    desc.functionPtr = &TestSpawnSystem::update;

    // System component reads.
    desc.componentReads.set(sge::ComponentIDCounter::get<sge::CGravity3>());
    desc.componentReads.set(sge::ComponentIDCounter::get<sge::CRigidBody3>());

    // No system component writes.

    // No system resource reads.
    
    // System resource writes.
    desc.resourceWrites.set(sge::ResourceIDCounter::get<sge::PlayerInputResource>());

    // System name.
    desc.name = "TestSpawnSystem";

    return desc;
}