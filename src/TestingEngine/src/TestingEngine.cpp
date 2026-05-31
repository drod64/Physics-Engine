#include <TestingEngine/TestingEngine.h>

TestingEngine::TestingEngine() :
TestingEngine("nothing")
{}

TestingEngine::TestingEngine(const std::string &filePath) :
sge::GameEngine()
{
    // TODO load level file stuff
    this->m_currentScene = "PLAY";

    this->changeScene<Scene_Play>(this->m_currentScene, filePath, false);

    InitWindow(800, 600, "Particle Physics Engine");
    SetTargetFPS(60);
}