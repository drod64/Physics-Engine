#include <TestingEngine/TestingEngine.h>

TestingEngine::TestingEngine() :
TestingEngine("nothing")
{}

void TestingEngine::init()
{
    sge::Registry &registry = this->currentScene()->getRegistry();
    sge::InputMappingResource &inputMapper = registry.getOrCreateResource<sge::InputMappingResource>();

    inputMapper.bindKey(KEY_W, static_cast<uint32_t>(ScenePlayAction::MoveForward));
    inputMapper.bindKey(KEY_A, static_cast<uint32_t>(ScenePlayAction::MoveLeft));
    inputMapper.bindKey(KEY_S, static_cast<uint32_t>(ScenePlayAction::MoveBackward));
    inputMapper.bindKey(KEY_D, static_cast<uint32_t>(ScenePlayAction::MoveRight));
    inputMapper.bindKey(KEY_SPACE, static_cast<uint32_t>(ScenePlayAction::MoveUp));
    inputMapper.bindKey(KEY_LEFT_CONTROL, static_cast<uint32_t>(ScenePlayAction::MoveDown));
    
    inputMapper.bindKey(KEY_Z, static_cast<uint32_t>(ScenePlayAction::SpawnAnchorBungee));
    inputMapper.bindKey(KEY_X, static_cast<uint32_t>(ScenePlayAction::SpawnAnchorSpring));
    inputMapper.bindKey(KEY_C, static_cast<uint32_t>(ScenePlayAction::SpawnBungeeSpring));
    inputMapper.bindKey(KEY_V, static_cast<uint32_t>(ScenePlayAction::SpawnBuoyant));
    inputMapper.bindKey(KEY_B, static_cast<uint32_t>(ScenePlayAction::SpawnFakeStiffSpring));
    inputMapper.bindKey(KEY_N, static_cast<uint32_t>(ScenePlayAction::SpawnSpring));
    
    inputMapper.bindKey(KEY_BACKSPACE, static_cast<uint32_t>(ScenePlayAction::DeleteEntities));

    // TODO create new resource for PlayerMouseInput (separated from PlayerInputResource).
    // inputMapper.bindMouse(MOUSE_BUTTON_LEFT, static_cast<uint32_t>(ScenePlayAction::MoveBackward));
    inputMapper.bindMouse(MOUSE_BUTTON_RIGHT, static_cast<uint32_t>(ScenePlayAction::RemoveGravity));
}

TestingEngine::TestingEngine(const std::string &filePath) :
sge::GameEngine()
{
    // TODO load level file stuff
    this->m_currentScene = "PLAY";

    this->changeScene<ScenePlay>(this->m_currentScene, filePath, false);

    init();

    InitWindow(800, 600, "Particle Physics Engine");
    ToggleFullscreen();
    SetTargetFPS(60);
}