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
        // TESTING ONLY.
        sge::PlayerActionSystem::update(this->getRegistry(), this->getCommandBuffer(), dt);
        TestSpawnSystem::update(this->getRegistry(), this->getCommandBuffer(), dt);
        this->m_world.update(dt);
    }
    
    if (this->hasEnded()) this->m_game->quit();
}

void ScenePlay::onEnd()
{
    this->m_hasEnded = true;
}

void ScenePlay::init()
{
    // Create camera entity.
    sge::Entity camera = this->getCommandBuffer().createEntityDeferred();
    sge::CTransform3 t3;
    sge::CCamera3 c3(true, CAMERA_PERSPECTIVE);
    
    // Add camera components to entity.
    this->getCommandBuffer().addComponentDeferred(camera, t3);
    this->getCommandBuffer().addComponentDeferred(camera, c3);
}