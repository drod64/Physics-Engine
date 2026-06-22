#include <SGE/core/Scene.h>
#include <SGE/core/GameEngine.h>

sge::Scene::Scene() :
Scene(nullptr)
{}

void sge::Scene::init()
{
    sge::SystemManager &sysManager = this->m_world.getSystemManager();
    // Register core physic systems.
    sysManager.registerSystem(sge::ClearAccumulatorsSystem3::getSystemDescriptor());
    sysManager.registerSystem(sge::GravitySystem3::getSystemDescriptor());
    sysManager.registerSystem(sge::AnchorBungeeSystem3::getSystemDescriptor());
    sysManager.registerSystem(sge::AnchorSpringSystem3::getSystemDescriptor());
    sysManager.registerSystem(sge::BungeeSpringSystem3::getSystemDescriptor());
    sysManager.registerSystem(sge::BuoyancySystem3::getSystemDescriptor());
    sysManager.registerSystem(sge::DragSystem3::getSystemDescriptor());
    sysManager.registerSystem(sge::FakeStiffSpringSystem3::getSystemDescriptor());
    sysManager.registerSystem(sge::SpringSystem3::getSystemDescriptor());
    sysManager.registerSystem(sge::IntegrationSystem3::getSystemDescriptor());
}

sge::Scene::Scene(sge::GameEngine *gameEngine) :
m_world(100)
{
    this->m_game = gameEngine;
    this->m_paused = false;
    this->m_hasEnded = false;
    this->m_currentFrame = 0;

    init();
}

void sge::Scene::setPaused(bool paused)
{
    this->m_paused = paused;
}

void sge::Scene::simulate(size_t frames)
{
    // TODO
}

size_t sge::Scene::getWidth() const
{
    return GetScreenWidth();
}

size_t sge::Scene::getHeight() const
{
    return GetScreenHeight();
}

size_t sge::Scene::getCurrentFrame() const
{
    return this->m_currentFrame;
}

bool sge::Scene::hasEnded() const
{
    return this->m_hasEnded;
}

sge::Registry& sge::Scene::getRegistry()
{
    return this->m_world.getRegistry();
}

sge::CommandBuffer& sge::Scene::getCommandBuffer()
{
    return this->m_world.getCommandBuffer();
}