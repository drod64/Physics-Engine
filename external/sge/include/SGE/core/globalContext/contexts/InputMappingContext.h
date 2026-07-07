#ifndef SGE_INPUT_MAPPING_CONTEXT_H
#define SGE_INPUT_MAPPING_CONTEXT_H
#include <algorithm>
#include <cstdint>
#include <vector>

namespace sge {
struct ButtonBind {
    int button;
    uint32_t actionID;

    ButtonBind()
    {
        this->button = 0;
        this->actionID = 0;
    }

    ButtonBind(int button, uint32_t actionID)
    {
        this->button = button;
        this->actionID = actionID;
    }
};
    
class InputMappingContext {
private:
    std::vector<ButtonBind> m_keyBindings;
    std::vector<ButtonBind> m_mouseBindings;

public:
    /**
     * Default construtor.
     */
    InputMappingContext();

    /**
     * Binds a key to an action.
     * @param key the key represented as an integer
     * @param actionID the action represented as an unsigned integer
     */
    void bindKey(int key, uint32_t actionID);

    /**
     * 
     * Retrieves a binded action ID when given a key.
     * @param key the key represented as an integer
     * @return the action ID represented as an unsigned integer
     */
    uint32_t getActionForKey(int key) const;
    
    /**
     * Retrieves all key-binded actions.
     * @return a std::vector of sge::ButtonBind objects
     */
    const std::vector<ButtonBind>& getKeyBinds() const;

    /**
     * Binds a mouse button to an action.
     * @param mouseButton the mouse button represented as an integer
     * @param actionID the action represented as an unsigned integer
     */
    void bindMouse(int mouseButton, uint32_t actionID);

    /**
     * Retrieves a binded action when given a mouse button.
     * @param mouseButton the mouse button represented as an integer
     * @return the action ID represented as an unsigned integer
     */
    uint32_t getActionForMouse(int mouseButton) const;

    /**
     * Retrieves all mouse-binded actions.
     * @return a std::vector of sge::ButtonBind objects
     */
    const std::vector<ButtonBind>& getMouseBinds() const;
}; // InputMappingContext
} // namespace sge

#endif // SGE_INPUT_MAPPING_CONTEXT_H