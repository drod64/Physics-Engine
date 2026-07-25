#include <TestingEngine/ScenePlay.h>

ScenePlay::ScenePlay() :
sge::Scene(nullptr)
{}

ScenePlay::ScenePlay(sge::GameEngine *gameEngine, const std::string &levelPath) :
sge::Scene(gameEngine)
{
    init();
}

void ScenePlay::update(float dt)
{
    if (!this->m_paused)
    {
        this->m_world.update(dt);
    }
    
    if (this->hasEnded())
    {
        this->m_game->quit();
    }
}

void ScenePlay::onEnd()
{
    this->m_hasEnded = true;
}

void ScenePlay::init()
{
    // Register required systems for World instance.
    auto &systemManager = this->m_world.getSystemManager();
    systemManager.registerSystem(CameraFollowSystem::getSystemDescription());
    systemManager.registerSystem(CameraMovementSystem::getSystemDescription());
    systemManager.registerSystem(InputDispatcherSystem::getSystemDescription());
    systemManager.registerSystem(PlayerMovementSystem::getSystemDescriptor());
    systemManager.registerSystem(TestSpawnSystem::getSystemDescriptor());
    // Compile systems.
    systemManager.compile();

    // Update camera entity (Camera entity already built with CTransform3 and CCamera3).
    sge::Entity camera = this->getRegistry().getContext<sge::CameraContext>().activeCamera;
    // Tag the camera as a First Person cam.
    this->getCommandBuffer().addComponentDeferred(camera, TagFPSCam());
    this->getCommandBuffer().addComponentDeferred(camera, CCameraControl3());

    sge::Entity playerID = spawnPlayer();

    CameraFollow3Command followCommand {
        .cameraEntity = camera,
        .targetEntity = playerID,
        .offset = {0, 1, 0}
    };

    // Link Camera to player.
    this->getCommandBuffer().pushCustomCommand(followCommand);
}

sge::Entity ScenePlay::spawnPlayer()
{
    sge::Entity player = this->getCommandBuffer().createEntityDeferred();
    sge::CTransform3 playerT3;
    playerT3.position = {0, 0, -40};
    playerT3.prevPosition = playerT3.position;

    sge::CRigidBody3 playerR3(10, false);

    sge::CPlayerController3 playerController;

    this->getCommandBuffer().addComponentDeferred(player, playerT3);
    this->getCommandBuffer().addComponentDeferred(player, playerR3);
    this->getCommandBuffer().addComponentDeferred(player, playerController);
    this->getCommandBuffer().addComponentDeferred(player, TagPlayer());

    return player;
}