#include <TestingEngine/systems/TestSpawnSystem.h>

void TestSpawnSystem::update(sge::Registry &registry, sge::CommandBuffer &cmdBuffer, sm::real dt)
{
    auto controllerView = registry.viewAll<sge::CPlayerController3>();
    auto gravity3View = registry.viewAll<sge::CGravity3>();

    for (const sge::Entity &e : controllerView)
    {
        const auto &controller = controllerView.get<sge::CPlayerController3>(e);

        for (const auto &actionEvent : controller.getActionEvents())
        {   
            if (actionEvent.type == sge::ActionType::Released) continue;

            switch (static_cast<ScenePlayAction>(actionEvent.actionID))
            {
                case ScenePlayAction::SpawnAnchorBungee:
                    ScenePlaySpawn::spawnAnchorBungee(cmdBuffer, {0,10.5,0});
                    break;

                case ScenePlayAction::SpawnAnchorSpring:
                    ScenePlaySpawn::spawnAnchorSpring(cmdBuffer, {10, 10, 0});
                    break;
                
                case ScenePlayAction::SpawnBuoyant:
                    ScenePlaySpawn::spawnBuoyancySpring(cmdBuffer);
                    break;

                case ScenePlayAction::RemoveGravity:
                    for (sge::Entity e : gravity3View)
                    {
                        cmdBuffer.removeComponentDeferred<sge::CGravity3>(e);
                    }
                    break;
                
                case ScenePlayAction::SpawnSpring:
                    ScenePlaySpawn::spawnSpringConnection(cmdBuffer);
                    break;

                case ScenePlayAction::SpawnBungeeSpring:
                    ScenePlaySpawn::spawnBungeeSpring(cmdBuffer);
                    break;
                    
                case ScenePlayAction::DeleteEntities:
                    for (sge::Entity e : gravity3View)
                    {
                        cmdBuffer.destroyEntityDeferred(e);
                    }
                    break;
                
                default:
                    break;
            }
        }
    }

    
}

sge::SystemDescriptor TestSpawnSystem::getSystemDescriptor()
{
    sge::SystemDescriptor desc;
    
    // System functor.
    desc.phase = sge::ExecutionPhase::PostGameplay;

    // System functor.
    desc.functionPtr = &TestSpawnSystem::update;

    // System component reads.
    desc.componentReads.set(sge::ComponentIDCounter::get<sge::CGravity3>());
    desc.componentReads.set(sge::ComponentIDCounter::get<sge::CRigidBody3>());
    desc.componentReads.set(sge::ComponentIDCounter::get<sge::CPlayerController3>());

    // No system component writes.

    // No system resource reads.
    
    // No system resource writes.

    // System name.
    desc.name = "TestSpawnSystem";

    return desc;
}