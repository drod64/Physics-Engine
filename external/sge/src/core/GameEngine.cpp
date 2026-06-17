#include <SGE/core/GameEngine.h>

namespace sge {

GameEngine::GameEngine()
{}

void GameEngine::update(float dt)
{
    sUserInput();
    currentScene()->update(dt);
}

void GameEngine::sUserInput()
{
    this->m_prevInput = this->m_curInput;
    
    updateInputSnapshot(this->m_curInput);

    currentScene()->handleInput(this->m_curInput, this->m_prevInput);

    if (WindowShouldClose())
    {
        quit();
    }

}

void sge::GameEngine::updateInputSnapshot(sge::InputSnapshot &input)
{
    // Check for keyboard input
    for (int i = 0; i < sge::InputSnapshot::INPUT_SIZE; ++i)
    {
        if (IsKeyDown(i))               { input.setKeyButton(i, true); }
        else if (IsKeyReleased(i))      { input.setKeyButton(i, false); }
    }

    // input.setKeyButton(KeyboardKey::KEY_P, IsKeyDown(KeyboardKey::KEY_P));

    // Check for mouse input
    for (int i = 0; i < sge::InputSnapshot::INPUT_SIZE; ++i)
    {
        input.setMouseButton(i, IsMouseButtonDown(i));
    }

    // Update mouse delta and position
    input.mouseDelta = sm::Vec2(GetMouseDelta().x, GetMouseDelta().y);
    input.mousePosition = sm::Vec2(GetMouseX(), GetMouseY());
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
