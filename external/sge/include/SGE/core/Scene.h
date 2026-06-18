#ifndef SGE_SCENE_H
#define SGE_SCENE_H
#include <memory>
#include <SGE/util/Action.h>
#include <SGE/core/World.h>
#include <raylib.h>


namespace sge {
// Forward declaration of GameEngine class.
class GameEngine;

/**
 * Base class that helps separate different gameplay logic and data in the form of scenes.
 * Sub-classes must inherit from this class and implement pure virtual functions.
 */
class Scene {
protected:
    GameEngine* m_game;
    sge::World m_world;
    bool m_paused;
    bool m_hasEnded;
    size_t m_currentFrame;

    /**
     * Pure virtual function that must be implemented by a sub-class.
     */
    virtual void onEnd() = 0;

    /**
     * Sets the scene as paused.
     * @param paused whether the scene is paused (true) or not (false)
     */
    void setPaused(bool paused);

    /**
     * Private Default Constructor.
     */
    Scene();

public:
    /**
     * Parameterized Constructor.
     * Each scene sub-class needs a valid pointer to a GameEngine object.
     * @param gameEngine the pointer of a GameEngine object
     */
    Scene(GameEngine *gameEngine);

    /**
     * Pure virtual function that must be implemented by a sub-class.
     * @param dt the delta time between frames
     */
    virtual void update(float dt) = 0;

    /**
     * TODO
     */
    void simulate(const size_t frames);
    
    /**
     * Returns the width of the rendering window.
     * @return the width of the window
     */
    size_t getWidth() const;

    /**
     * Returns the height of the rendering window.
     * @return the height of the window
     */
    size_t getHeight() const;

    /**
     * Returns the current frame count.
     * @return the frame count
     */
    size_t getCurrentFrame() const;

    /**
     * Checks if the scene has ended.
     * @return true if the scene has ended, false otherwise
     */
    bool hasEnded() const;

    virtual Registry& getRegistry();

    virtual CommandBuffer& getCommandBuffer();
};

}

#endif // SGE_SCENE_H