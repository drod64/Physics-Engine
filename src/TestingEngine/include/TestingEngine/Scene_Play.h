#ifndef SCENE_PLAY_H
#define SCENE_PLAY_H
#include <assert.h>
#include <string>
#include <SGE/entities/Entity.h>
#include <SGE/util/Scene.h>
#include <SGE/util/GameEngine.h>
#include <SGE/util/InputSnapshot.h>
#include <SGE/systems/physics/ForceRegistry3.h>
#include <SGE/systems/physics/GravityForce3.h>
#include <SGE/systems/physics/DragForce3.h>
#include <SGE/systems/physics/SpringForce3.h>
#include <TestingEngine/Action_Scene_Play.h>
#include <TestingEngine/ProjectileType.h>
#include <raylib.h>

class Scene_Play : public sge::Scene {
private:
    // Testing ForceRegistry and ForceGenerators
    sge::ForceRegistry3 m_registry;
    sge::GravityForce3 m_gravity;
    sge::DragForce3 m_drag;
    // Testing SpringForces
    std::shared_ptr<sge::SpringForce3> m_spring1;
    std::shared_ptr<sge::SpringForce3> m_spring2;
    // End "Testing SpringForces"

    
    bool m_drawGrid;
    Camera3D m_camera;

    /////////////////////////////
    //   Overriden functions   //
    /////////////////////////////
    /**
     * Handles the updating logic for the Scene_Play class.
     * @param dt the time lapsed since the last frame
     */
    void update(float dt) override;
    
    void handleInput(const sge::InputSnapshot &curInput, const sge::InputSnapshot &prevInput) override;

    /**
     * Keeps track of which actions should be done (depending on whether key was released or pressed).
     * @param action data that contains whether the key was pressed or released [action.getType()]
     * and the action name (which tells the scene which specific logic to carry out next)
     */
    void sDoAction(const sge::Action &action) override;

    /**
     * Handles the rendering logic of the Scene_Play class.
     */
    void sRender() override;

    /**
     * Ends the current scene.
     */
    void onEnd() override;

    ////////////////////
    //     Other      //
    ////////////////////
    /**
     * 
     */
    void init();

    void spawnProjectile(ProjectileType type);

    void spawnFirework(const std::string &tag, const sm::Vec3 &position, const sm::Vec3 &velocity, int lifespan);

    void spawnSpringConnection();

    /////////////////////
    //     Systems     //
    /////////////////////
    /**
     * Handles movement logic for the Scene_Play class.
     */
    void sMovement(sm::real dt);

    /**
     * Handles life span logic for the Scene_Play class.
     */
    void sLifeSpan(sm::real dt);

    /**
     * Private default constructor
     */
    Scene_Play();

public:
    /** 
     * Scene_Play Constructor
     * @param gameEngine an exisiting gameEngine pointer (associated with this scene)
     * @param levelPath a file path to the file containing data on level generation
     */
    Scene_Play(sge::GameEngine *gameEngine, const std::string &levelPath);

};

#endif // SCENE_PLAY_H