#include <SGE/util/Scene.h>
#include <SGE/util/GameEngine.h>

sge::Scene::Scene() :
Scene(nullptr)
{}

sge::Scene::Scene(sge::GameEngine *gameEngine)
{
    this->m_game = gameEngine;
    this->m_paused = false;
    this->m_hasEnded = false;
    this->m_currentFrame = 0;
}

void sge::Scene::setPaused(bool paused)
{
    this->m_paused = paused;
}

void sge::Scene::doAction(const Action &action)
{
    this->sDoAction(action);
}

void sge::Scene::simulate(size_t frames)
{
    // TODO
}

void sge::Scene::registerAction(int inputKey, sge::ActionID actionID)
{
    this->m_actionMap[inputKey] = actionID;
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

const std::map<int, sge::ActionID>& sge::Scene::getActionMap() const
{
    return this->m_actionMap;
}

Mesh sge::Scene::createLine(const sm::Vec2 &p1, const sm::Vec2 &p2)
{
    
}