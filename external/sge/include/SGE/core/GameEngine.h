#ifndef SGE_GAME_ENGINE_H
#define SGE_GAME_ENGINE_H
// STD includes
#include <iostream>
#include <string>
#include <unordered_map>
#include <chrono>
#include <thread>
#include <raylib.h>

// Custom library includes
#include <SGE/core/Scene.h>
#include <SGE/systems/PollInputSystem.h>

// Necessary systems
#include <SGE/systems/RenderingSystem3.h>
#include <SGE/managers/InputActionSnapshot.h>

namespace sge {
/**
 * Class that stores and updates game data.
 */
class GameEngine {
protected:
    std::string m_currentScene;
    std::unordered_map<std::string, std::shared_ptr<sge::Scene>> m_scenes;
    bool m_running;
    sm::real m_accumulator_dt;
    const sm::real m_FIXED_SIMULATION_DT; 

    /**
     * Default Constructor.
     */
    GameEngine();

    /**
     * Retrives the current scene the game engine is on.
     * @return a std::shared_ptr<sge::Scene> to the current scene
     */
    std::shared_ptr<sge::Scene> currentScene();

public:
    /**
     * Changes the scene of the game engine.
     * @param sceneName the name of the desired scene (if it does not exist, it will be created).
     * @param levelPath the file path to level data
     * @param endCurrentScene whether to end the current scene before changing to the new scene (false by default)
     */
    template <typename T>
    void changeScene(const std::string &sceneName, const std::string &levelPath, bool endCurrentScene = false)
    {
        // Remove Scene from map.
        if (endCurrentScene)
        {
            auto it = this->m_scenes.find(sceneName);
            if (it != this->m_scenes.end())
            this->m_scenes.erase(it);
        }
        // If Scene does not exist, create it.
        if (this->m_scenes.find(sceneName) == this->m_scenes.end())
        {
            std::cout << "Creating scene: " << sceneName << '\n';
            std::shared_ptr<sge::Scene> scene = std::make_shared<T>(this, levelPath);
            this->m_scenes[sceneName] = scene;
        }
        std::cout << "Changed scene\n";
        this->m_currentScene = sceneName;
    }

    /**
     * Ends the game engine from running.
     */
    void quit();

    /**
     * Starts the game engine loop.
     */
    void run();

    /**
     * Checks if the game engine is running.
     * @return true if the engine is running, false otherwise
     */
    bool isRunning() const;
};

}

#endif // SGE_GAME_ENGINE_H