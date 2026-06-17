#ifndef SCENE_PLAY_H
#define SCENE_PLAY_H
#include <assert.h>
#include <string>
#include <SGE/entities/Entity.h>
#include <SGE/core/Scene.h>
#include <SGE/core/GameEngine.h>
#include <SGE/core/InputSnapshot.h>
#include <SGE/core/World.h>
#include <TestingEngine/ScenePlaySpawn.h>
#include <raylib.h>

class ScenePlay : public sge::Scene {
private:
    sge::World m_world;
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

    /////////////////////
    //     Systems     //
    /////////////////////

    /**
     * Handles life span logic for the Scene_Play class.
     */
    void sLifeSpan(sm::real dt);

    /**
     * Private default constructor
     */
    ScenePlay();

public:
    /** 
     * Scene_Play Constructor
     * @param gameEngine an exisiting gameEngine pointer (associated with this scene)
     * @param levelPath a file path to the file containing data on level generation
     */
    ScenePlay(sge::GameEngine *gameEngine, const std::string &levelPath);

};

#endif // SCENE_PLAY_H