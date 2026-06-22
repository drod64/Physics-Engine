#ifndef SGE_INPUT_MAPPING_RESOURCE_H
#define SGE_INPUT_MAPPING_RESOURCE_H
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
    
class InputMappingResource {
private:
    std::vector<ButtonBind> m_keyBindings;
    std::vector<ButtonBind> m_mouseBindings;

public:
    InputMappingResource();
    void bindKey(int key, uint32_t actionID);
    uint32_t getActionForKey(int key) const;
    const std::vector<ButtonBind>& getKeyBinds() const;

    void bindMouse(int mouseButton, uint32_t actionID);
    uint32_t getActionForMouse(int mouseButton) const;
    const std::vector<ButtonBind>& getMouseBinds() const;
};
} // namespace sge

#endif // SGE_INPUT_MAPPING_RESOURCE_H