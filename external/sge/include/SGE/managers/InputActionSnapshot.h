#ifndef SGE_INPUT_SNAPSHOT_H
#define SGE_INPUT_SNAPSHOT_H
#include <assert.h>
#include <vector>
#include <sm/Vec2.h>
#include <bitset>

namespace sge {
/**
 * Helper class that assists with storing action states.
 * This object should be fed by the InputSystem (which reads raw key/mouse button presses).
 */
class InputActionSnapshot {
private:
    static const uint32_t MAX_ACTIONS = 512;
    std::bitset<MAX_ACTIONS> m_mask;

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
    void setAction(uint32_t actionID, bool value);

    /**
     * Retrieves the state of the desired action state.
     * @param action the action state to check
     * @return whether the action is active or not
     */
    bool getActionState(uint32_t actionID) const;

    /**
     * Resets the snapshot to 0.
     */
    void reset();
};
} // namespace sge

#endif // SGE_INPUT_SNAPSHOT_H