#ifndef TESTING_ENGINE_H
#define TESTING_ENGINE_H
#include <SGE/util/GameEngine.h>
#include <TestingEngine/Scene_Play.h>
#include <string>


// TODO: Will need to focus on implementing Physics, Animation, Scene, and Action classes in the game engine.
// This is the game engine for TestingEngine
class TestingEngine : public sge::GameEngine {
private:
    TestingEngine();

public:
    TestingEngine(const std::string &path);

};

#endif // TESTING_ENGINE_H