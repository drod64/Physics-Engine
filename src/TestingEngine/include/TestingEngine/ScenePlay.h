#ifndef SCENE_PLAY_H
#define SCENE_PLAY_H
#include <assert.h>
#include <string>
#include <raylib.h>
#include <SGE/core/GameEngine.h>
#include <SGE/core/Scene.h>
#include <SGE/core/ecs/Entity.h>
#include <SGE/core/ecs/components/CCamera3.h>
#include <TestingEngine/components/TagPlayer.h>
#include <TestingEngine/ScenePlaySpawn.h>
#include <TestingEngine/components/TagFPSCam.h>
#include <TestingEngine/components/CCameraControl3.h>
#include <TestingEngine/commands/CameraFollow3Command.h>

// Testing systems with SystemManager implementation.
#include <TestingEngine/systems/CameraFollowSystem.h>
#include <TestingEngine/systems/CameraMovementSystem.h>
#include <TestingEngine/systems/InputDispatcherSystem.h>
#include <TestingEngine/systems/PlayerMovementSystem.h>
#include <TestingEngine/systems/TestSpawnSystem.h>

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

    sge::Entity spawnPlayer();

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