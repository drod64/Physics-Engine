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
    systemManager.registerSystem(InputDispatcherSystem::getSystemDescription());
    systemManager.registerSystem(TestSpawnSystem::getSystemDescriptor());
    systemManager.registerSystem(PlayerMovementSystem::getSystemDescriptor());
    // Compile systems.
    systemManager.compile();

    // Create camera entity.
    sge::Entity camera = this->getCommandBuffer().createEntityDeferred();
    sge::CTransform3 t3;
    sge::CRigidBody3 r3;
    r3.setMass(10);
    r3.setStatic(false);
    sge::CCamera3 c3(true, CAMERA_PERSPECTIVE);
    sge::CPlayerController3 controller;
    t3.position = {0, 0, -40};
    // Add camera components to entity.
    this->getCommandBuffer().addComponentDeferred(camera, t3);
    this->getCommandBuffer().addComponentDeferred(camera, r3);
    this->getCommandBuffer().addComponentDeferred(camera, c3);
    this->getCommandBuffer().addComponentDeferred(camera, controller);
    this->getCommandBuffer().addComponentDeferred(camera, sge::CGravity3({0, -9.81, 0}));
}