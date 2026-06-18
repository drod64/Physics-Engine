#ifndef SGE_INPUT_PROVIDER_H
#define SGE_INPUT_PROVIDER_H
#include <unordered_map>
#include <raylib.h>
#include <SGE/util/Action.h>
#include <SGE/core/View.h>
#include <SGE/core/CommandBuffer.h>
#include <SGE/core/InputActionSnapshot.h>
#include <SGE/core/resources/PlayerInputResource.h>

// 1. translation, intention, and execution pipeline
// InputProvider -> PlayerActionSystem -> PlayerMovementSystem or any other system that depends on a specific action.
namespace sge {
class InputProvider {
private:
    InputActionSnapshot m_curInput;
    std::unordered_map<int, sge::Action> m_keyToAction;
    std::unordered_map<int, sge::Action> m_mouseToAction;

    /**
     * Updates the input snapshot (state) of the game engine by reading directly
     * from keyboard/mouse button presses/releases.
     * @param input the InputSnapshot to update
     */
    void updateInputSnapshot(InputActionSnapshot &input);

public:
    InputProvider();

    /**
     * Functor
     */
    void operator()(Registry &registry, CommandBuffer &, sm::real);

    const InputActionSnapshot& getCurrentSnapshot() const;

    void bindKey(int key, Action action);

    void bindMouse(int mouse, Action action);
};
}

#endif // SGE_INPUT_PROVIDER_H