#include <SGE/core/GameEngine.h>

namespace sge {

GameEngine::GameEngine()
{}

void GameEngine::update(float dt)
{
    // Call input system.
    inputSystem(currentScene()->getRegistry(), currentScene()->getCommandBuffer(), dt);

    // Update current scene.
    // All registered systems pertaining to the current scene will be invoked.
    currentScene()->update(dt);

    // Finally, render current frame.
    sge::RenderingSystem3::update(currentScene()->getRegistry(), currentScene()->getCommandBuffer(), dt);

    if (WindowShouldClose())
    {
        quit();
    }
}

std::shared_ptr<sge::Scene> GameEngine::currentScene()
{
    return this->m_scenes.at(this->m_currentScene);
}

void GameEngine::quit()
{
    this->m_running = false;
}

void GameEngine::run()
{
    while (this->m_running)
    {
        update(GetFrameTime());
    }

    CloseWindow();
}

bool GameEngine::isRunning() const
{
    return this->m_running;
}

} // End namespace SGE
