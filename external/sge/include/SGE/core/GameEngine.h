#ifndef SGE_GAME_ENGINE_H
#define SGE_GAME_ENGINE_H
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
// Library includes
#include <SGE/entities/EntityManager.h>
#include <SGE/core/Scene.h>
#include <SGE/core/InputSnapshot.h>
#include <raylib.h>

namespace sge {
/**
 * Class that stores and updates game data.
 */
class GameEngine {
protected:
    std::string m_currentScene;
    std::unordered_map<std::string, std::shared_ptr<sge::Scene>> m_scenes; 
    size_t m_simulationSpeed;
    bool m_running;
    InputSnapshot m_curInput;
    InputSnapshot m_prevInput;

    /**
     * Default Constructor.
     */
    GameEngine();

    /**
     * Updates the game by gathering user input and updating the current scene.
     * @param dt the delta time between frames
     */
    void update(float dt);

    /**
     * Stores the previous input state while also updating the current input state.
     * Also calls the current scene to handle the passed in previous input state and current input state.
     */
    void sUserInput();

    /**
     * Updates the input snapshot (state) of the game engine by reading directly
     * from keyboard/mouse button presses/releases.
     * @param input the InputSnapshot to update
     */
    void updateInputSnapshot(InputSnapshot &input);

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