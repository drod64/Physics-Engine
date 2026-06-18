#ifndef SCENE_PLAY_H
#define SCENE_PLAY_H
#include <assert.h>
#include <string>
#include <SGE/entities/Entity.h>
#include <SGE/core/Scene.h>
#include <SGE/core/GameEngine.h>
#include <TestingEngine/ScenePlaySpawn.h>
#include <SGE/components/CCamera3.h>
#include <SGE/systems/PlayerActionSystem.h>
#include <TestingEngine/systems/TestSpawnSystem.h>
#include <raylib.h>

class ScenePlay : public sge::Scene {
private:
    /////////////////////////////
    //   Overriden functions   //
    /////////////////////////////
    /**
     * Handles the updating logic for the Scene_Play class.
     * @param dt the time lapsed since the last frame
     */
    void update(float dt) override;

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