#ifndef SGE_INPUT_SNAPSHOT_H
#define SGE_INPUT_SNAPSHOT_H
#include <assert.h>
#include <vector>
#include <sm/Vec2.h>
#include <SGE/util/Action.h>
#include <SGE/util/Bitmask.h>

namespace sge {
/**
 * Helper class that assists with storing action states.
 * This object should be fed by the InputSystem (which reads raw key/mouse button presses).
 */
class InputActionSnapshot {
private:
    std::vector<Bitmask> m_mask;
    static const int INPUT_SIZE;

public:
    sm::Vec2 mouseDelta;
    sm::Vec2 mousePosition;

    /**
     * Default Constructor.
     */
    InputActionSnapshot();

    /**
     * Copy Constructor.
     * @param otherInput the other InputSnapshot to copy
     */
    InputActionSnapshot(const InputActionSnapshot &otherInput);

    /**
     * Overload of assignment operator.
     * @param otherInput the other InputSnapshot to copy
     * @return a reference to *this
     */
    InputActionSnapshot& operator = (const InputActionSnapshot &otherInput);

    /**
     * Sets a specific action state to either true or false.
     * @param action the action state to update
     * @param value whether the action is active or not
     */
    void setAction(Action action, bool value);

    /**
     * Retrieves the state of the desired action state.
     * @param action the action state to check
     * @return whether the action is active or not
     */
    bool getActionState(Action action) const;

    /**
     * Resets the snapshot to 0.
     */
    void clear();
};
} // namespace sge

#endif // SGE_INPUT_SNAPSHOT_H