#ifndef SGE_INPUT_SNAPSHOT_H
#define SGE_INPUT_SNAPSHOT_H
#include <assert.h>
#include <vector>
#include <sm/Vec2.h>
#include <SGE/util/Bitmask.h>

namespace sge {
/**
 * Helper class that assists with storing input state such as keyboard and mouse button presses/releases.
 */
class InputSnapshot {
private:
    std::vector<Bitmask> m_keyButtons;
    std::vector<Bitmask> m_mouseButtons;

public:
    static const int INPUT_SIZE;
    sm::Vec2 mouseDelta;
    sm::Vec2 mousePosition;

    /**
     * Default Constructor.
     */
    InputSnapshot();

    /**
     * Copy Constructor.
     * @param otherInput the other InputSnapshot to copy
     */
    InputSnapshot(const InputSnapshot &otherInput);

    /**
     * Overload of assignment operator.
     * @param otherInput the other InputSnapshot to copy
     * @return a reference to *this
     */
    InputSnapshot& operator = (const InputSnapshot &otherInput);

    /**
     * Sets a specific key button to pressed/released.
     * @param keyButton the key button to update
     * @param value whether the button is pressed(true) or released(false)
     */
    void setKeyButton(int keyButton, bool value);

    /**
     * Sets a specific mouse button to pressed/released.
     * @param mouseButton the mouse button to update
     * @param value whether the button is pressed(true) or released(false)
     */
    void setMouseButton(int mouseButton, bool value);

    /**
     * Retrieves the state of the desired key button.
     * @param keyButton the key button to check
     * @return whether the button is pressed(true) or released(false)
     */
    bool getKeyButtonState(int keyButton) const;

    /**
     * Retrieves the state of the desired mouse button.
     * @param mouseButton the mouse button to check
     * @return whether the button is pressed(true) or released(false)
     */
    bool getMouseButtonState(int mouseButton) const;

};
} // namespace sge

#endif // SGE_INPUT_SNAPSHOT_H