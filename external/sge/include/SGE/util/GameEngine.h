#ifndef SGE_GAME_ENGINE_H
#define SGE_GAME_ENGINE_H
#include <iostream>
#include <memory>
#include <string>
#include <map>
// Library includes
#include <SGE/entities/EntityManager.h>
#include <SGE/util/Scene.h>
#include <SGE/util/InputSnapshot.h>
#include <raylib.h>

namespace sge {

class GameEngine {
protected:
    std::string m_currentScene;
    std::unordered_map<std::string, std::shared_ptr<sge::Scene>> m_scenes; 
    size_t m_simulationSpeed;
    bool m_running;
    InputSnapshot m_curInput;
    InputSnapshot m_prevInput;

    GameEngine();
    void update(float dt);
    void sUserInput();

    void updateInputSnapshot(InputSnapshot &input);
    std::shared_ptr<sge::Scene> currentScene();

public:
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

    void quit();
    void run();

    bool isRunning() const;
};

}

#endif // SGE_GAME_ENGINE_H