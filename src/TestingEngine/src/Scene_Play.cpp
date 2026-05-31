#include <TestingEngine/Scene_Play.h>

Scene_Play::Scene_Play() :
sge::Scene(nullptr)
{}

Scene_Play::Scene_Play(sge::GameEngine *gameEngine, const std::string &levelPath) :
sge::Scene(gameEngine)
{
    
    this->m_drawGrid = false;
    init();
}

void Scene_Play::update(float dt)
{
    this->m_entities.update();

    if (!this->m_paused)
    {
        this->sMovement(dt);
        this->sLifeSpan(dt);
    }
    this->sRender();

    if (this->hasEnded()) this->m_game->quit();
}

void Scene_Play::handleInput(const sge::InputSnapshot &curInput, const sge::InputSnapshot &prevInput)
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

void Scene_Play::sDoAction(const sge::Action &action)
{
    if (action.getType() == sge::ActionType::START)
    {
        // Toggle / Single press actions
            if (action.getID() == GameplayAction::TOGGLE_GRID)         { this->m_drawGrid = !this->m_drawGrid; }
        else if (action.getID() == GameplayAction::PAUSE)               { this->m_paused = !this->m_paused; }
        else if (action.getID() == GameplayAction::QUIT)                { this->onEnd(); }
        else if (action.getID() == GameplayAction::SHOOT)               { spawnBall(); }
    }
    else if (action.getType() == sge::ActionType::END)
    {
        
    }
}

void Scene_Play::sRender()
{
    UpdateCamera(&this->m_camera, CAMERA_FREE);

    // Color the background darker to indicate game is paused.
    if (!this->m_paused) { ClearBackground(RAYWHITE); }
    else { ClearBackground(Color{50, 50, 150}); }
    
    BeginDrawing();
    
    BeginMode3D(this->m_camera);

    if (this->m_drawGrid) DrawGrid(10, 1.0f);

    for (const auto &e : this->m_entities.getEntities())
    {
        if (e->hasComponent<sge::CTransform3>())
        {
            const sge::CTransform3 &t3 = e->getComponent<sge::CTransform3>();

            DrawCircle3D({t3.position.x, t3.position.y, t3.position.z}, 2, {1, t3.rotation.y, t3.rotation.z}, 90, RED);
        }
    }

    EndMode3D();

    EndDrawing();
}

void Scene_Play::onEnd()
{
    this->m_hasEnded = true;
}

void Scene_Play::init()
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
    this->registerAction(KeyboardKey::KEY_Z,        GameplayAction::SHOOT);

    // Initializing Camera3D
    this->m_camera.position = {0, 5, 5};
    this->m_camera.fovy = 45.f;
    this->m_camera.target = {0, 0, 0};
    this->m_camera.projection = CAMERA_PERSPECTIVE;
    this->m_camera.up = {0, 1, 0};
}

void Scene_Play::spawnBall()
{
    auto e = this->m_entities.addEntity("ball");

    e->addComponent<sge::CTransform3>();
    e->addComponent<sge::CRigidBody3>(sm::Vec3(50,50,0), sm::Vec3(25,25,0), 50, 0.5);
    e->addComponent<sge::CLifespan>(600);
}

void Scene_Play::sMovement(sm::real dt)
{
    // Update all entity positions with their velocities and accelerations
    for (auto &e : this->m_entities.getEntities())
    {
        if (e->hasComponent<sge::CRigidBody3>() &&
            e->hasComponent<sge::CTransform3>())
        {
            sge::CRigidBody3 &r3 = e->getComponent<sge::CRigidBody3>();
            sge::CTransform3 &t3 = e->getComponent<sge::CTransform3>();

            if (r3.getMass() <= 0.0f) return;

            assert(dt > 0.0f);
            
            r3.accumulatedForce += sm::Vec3(0, -9.81 * r3.getMass(), 0);
            
            // Update position
            t3.position.addScaledVector(r3.velocity, dt);

            // Calculate acceleration (dependending on acting forces)
            // and Update velocity
            r3.velocity.addScaledVector(r3.accumulatedForce * (1.0 / r3.getMass()), dt);

            // Apply damping to the velocity
            r3.velocity *= real_pow(r3.damping, dt);
        }
    }
}

void Scene_Play::sLifeSpan(sm::real dt)
{
    for (auto &e : this->m_entities.getEntities())
    {
        if (e->hasComponent<sge::CLifespan>())
        {
            // If the lifespan has been reached, destroy the entity
            if (e->getComponent<sge::CLifespan>().remaining <= 0)
            {
                e->destroy();
            }
            // Otherwise decrement the remaining frame count
            else
            {
                e->getComponent<sge::CLifespan>().remaining -= dt;
            }
        }
    }
}