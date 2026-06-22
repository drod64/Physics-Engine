#ifndef TESTING_ENGINE_H
#define TESTING_ENGINE_H
#include <string>
#include <SGE/core/GameEngine.h>
#include <TestingEngine/ScenePlay.h>
#include <TestingEngine/ScenePlayActions.h>


// TODO: Will need to focus on implementing Physics, Animation, Scene, and Action classes in the game engine.
// This is the game engine for TestingEngine
class TestingEngine : public sge::GameEngine {
private:
    TestingEngine();
    void init();

public:
    TestingEngine(const std::string &path);


};

#endif // TESTING_ENGINE_H