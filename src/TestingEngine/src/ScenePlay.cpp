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

    auto &systemOrder = this->m_world.getSystemManager().getExecutionOrder();
    
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
    systemManager.registerSystem(PlayerActionSystem::getSystemDescription());
    systemManager.registerSystem(TestSpawnSystem::getSystemDescriptor());
    // Compile systems.
    systemManager.compile();

    // Create camera entity.
    sge::Entity camera = this->getCommandBuffer().createEntityDeferred();
    sge::CTransform3 t3;
    sge::CCamera3 c3(true, CAMERA_PERSPECTIVE);
    // Add camera components to entity.
    this->getCommandBuffer().addComponentDeferred(camera, t3);
    this->getCommandBuffer().addComponentDeferred(camera, c3);
}