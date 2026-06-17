#include <TestingEngine/ScenePlay.h>

ScenePlay::ScenePlay() :
sge::Scene(nullptr),
m_world(100)
{}

ScenePlay::ScenePlay(sge::GameEngine *gameEngine, const std::string &levelPath) :
sge::Scene(gameEngine),
m_world(100)
{
    this->m_drawGrid = false;
    init();
}

void ScenePlay::update(float dt)
{
    if (!this->m_paused)
    {
        this->m_world.update(dt);
    }
    this->sRender();

    if (this->hasEnded()) this->m_game->quit();
}

void ScenePlay::handleInput(const sge::InputSnapshot &curInput, const sge::InputSnapshot &prevInput)
{
    for (const auto &action_item : this->m_actionMap)
    {
        // Button pressed
        if (!prevInput.getKeyButtonState(action_item.first) && curInput.getKeyButtonState(action_item.first))
        {
            this->doAction(sge::Action(action_item.second, sge::ActionType::START));
        }
        // Button held
        else if (prevInput.getKeyButtonState(action_item.first) && curInput.getKeyButtonState(action_item.first))
        {
            this->doAction(sge::Action(action_item.second, sge::ActionType::HELD));
        }
        // Button released
        else if (prevInput.getKeyButtonState(action_item.first) && !curInput.getKeyButtonState(action_item.first))
        {
            this->doAction(sge::Action(action_item.second, sge::ActionType::END));
        }
    }
}

void ScenePlay::sDoAction(const sge::Action &action)
{
    if (action.getType() == sge::ActionType::START)
    {
        // Toggle / Single press actions
            if (action.getID() == GameplayAction::TOGGLE_GRID)              { this->m_drawGrid = !this->m_drawGrid; }
        else if (action.getID() == GameplayAction::PAUSE)                   { this->m_paused = !this->m_paused; }
        else if (action.getID() == GameplayAction::QUIT)                    { this->onEnd(); }
        else if (action.getID() == GameplayAction::SHOOT_PISTOL)            { ScenePlaySpawn::spawnProjectile(this->m_world.getCommandBuffer(), ProjectileType::PISTOL); }
        else if (action.getID() == GameplayAction::SHOOT_ARTILLERY)         { ScenePlaySpawn::spawnProjectile(this->m_world.getCommandBuffer(), ProjectileType::ARTILLERY); }
        else if (action.getID() == GameplayAction::SHOOT_FIREBALL)          { ScenePlaySpawn::spawnProjectile(this->m_world.getCommandBuffer(), ProjectileType::FIREBALL); }
        else if (action.getID() == GameplayAction::SHOOT_LASER)             { ScenePlaySpawn::spawnProjectile(this->m_world.getCommandBuffer(), ProjectileType::LASER); }
        else if (action.getID() == GameplayAction::SHOOT_FIREWORK)          { ScenePlaySpawn::spawnFirework(this->m_world.getCommandBuffer(), "initial-firework"); }
        else if (action.getID() == GameplayAction::SPAWN_SPRING)            { ScenePlaySpawn::spawnSpringConnection(this->m_world.getCommandBuffer()); }
        else if (action.getID() == GameplayAction::SPAWN_ANCHOR_SPRING)     { ScenePlaySpawn::spawnAnchorSpring(this->m_world.getCommandBuffer(), {0, 0, 0}); }
        else if (action.getID() == GameplayAction::SPAWN_BUNGEE_SPRING)     { ScenePlaySpawn::spawnBungeeSpring(this->m_world.getCommandBuffer()); }
        else if (action.getID() == GameplayAction::SPAWN_BUOYANCY_SPRING)   { ScenePlaySpawn::spawnBuoyancySpring(this->m_world.getCommandBuffer()); }
        else if (action.getID() == GameplayAction::SPAWN_FAKE_SPRING)       { ScenePlaySpawn::spawnFakeSpring(this->m_world.getCommandBuffer()); }
    }
    else if (action.getType() == sge::ActionType::END)
    {
        
    }
}

void ScenePlay::sRender()
{
    UpdateCamera(&this->m_camera, CAMERA_FREE);

    // Color the background darker to indicate game is paused.
    if (!this->m_paused) { ClearBackground(RAYWHITE); }
    else { ClearBackground(Color{50, 50, 150}); }
    
    BeginDrawing();
    
    BeginMode3D(this->m_camera);

    if (this->m_drawGrid) DrawGrid(10, 1.0f);

    // Component pool of sge::CTransform3
    auto &t3Components = this->m_world.getRegistry().getPool<sge::CTransform3>()->getDenseComponents();

    for (const auto &t3 : t3Components)
    {
        DrawCube({t3.position.x, t3.position.y, t3.position.z}, 2, 2, 2, RED);
    }

    EndMode3D();

    EndDrawing();
}

void ScenePlay::onEnd()
{
    this->m_hasEnded = true;
}

void ScenePlay::init()
{
    // Basic actions
    this->registerAction(KeyboardKey::KEY_P,        GameplayAction::PAUSE);
    this->registerAction(KeyboardKey::KEY_ESCAPE,   GameplayAction::QUIT);
    this->registerAction(KeyboardKey::KEY_G,        GameplayAction::TOGGLE_GRID);

    // Movement actions
    this->registerAction(KeyboardKey::KEY_LEFT,     GameplayAction::LEFT);
    this->registerAction(KeyboardKey::KEY_UP,       GameplayAction::JUMP);
    this->registerAction(KeyboardKey::KEY_RIGHT,    GameplayAction::RIGHT);
    this->registerAction(KeyboardKey::KEY_DOWN,     GameplayAction::DOWN);

    // Spawning actions
    this->registerAction(KeyboardKey::KEY_Z,        GameplayAction::SHOOT_PISTOL);
    this->registerAction(KeyboardKey::KEY_X,        GameplayAction::SHOOT_ARTILLERY);
    this->registerAction(KeyboardKey::KEY_C,        GameplayAction::SHOOT_FIREBALL);
    this->registerAction(KeyboardKey::KEY_V,        GameplayAction::SHOOT_LASER);
    this->registerAction(KeyboardKey::KEY_B,        GameplayAction::SHOOT_FIREWORK);
    this->registerAction(KeyboardKey::KEY_N,        GameplayAction::SPAWN_SPRING);
    this->registerAction(KeyboardKey::KEY_M,        GameplayAction::SPAWN_ANCHOR_SPRING);
    this->registerAction(KeyboardKey::KEY_Q,        GameplayAction::SPAWN_BUNGEE_SPRING);
    this->registerAction(KeyboardKey::KEY_E,        GameplayAction::SPAWN_BUOYANCY_SPRING);
    this->registerAction(KeyboardKey::KEY_R,        GameplayAction::SPAWN_FAKE_SPRING);

    // Initializing Camera3D
    this->m_camera.position = {0, 5, 5};
    this->m_camera.fovy = 45.f;
    this->m_camera.target = {0, 0, 0};
    this->m_camera.projection = CAMERA_PERSPECTIVE;
    this->m_camera.up = {0, 1, 0};
}

// void Scene_Play::sLifeSpan(sm::real dt)
// {
//     for (auto &e : this->m_entities.getEntities())
//     {
//         if (e->hasComponent<sge::CLifespan>())
//         {
//             // If the lifespan has been reached, destroy the entity
//             if (e->getComponent<sge::CLifespan>().remaining <= 0)
//             {
//                 if (e->tag() == "initial-firework")
//                 {
//                     spawnFirework("explosion", e->getComponent<sge::CTransform3>().position, sm::Vec3(10, 10, 0), 60);
//                     spawnFirework("explosion", e->getComponent<sge::CTransform3>().position, sm::Vec3(-10, 10, 0), 60);
//                     spawnFirework("explosion", e->getComponent<sge::CTransform3>().position, sm::Vec3(0, 10, 10), 60);
//                     spawnFirework("explosion", e->getComponent<sge::CTransform3>().position, sm::Vec3(0, 10, -10), 60);
//                 }
                
//                 // Remove entity from force registry
//                 this->m_registry.removeEntity(e.get());

//                 // Mark entity to be destroyed (by EntityManager)
//                 e->destroy();
//             }
//             // Otherwise decrement the remaining frame count
//             else
//             {
//                 e->getComponent<sge::CLifespan>().remaining -= dt;
//             }
//         }
//     }
// }