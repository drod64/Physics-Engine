#include <SGE/core/globalContext/contexts/InputMappingContext.h>

sge::InputMappingContext::InputMappingContext()
{
    // Average key buttons used.
    this->m_keyBindings.reserve(120);
    // Average mouse buttons used (with padding).
    this->m_mouseBindings.reserve(16);
}

void sge::InputMappingContext::bindKey(int key, uint32_t actionID)
{
    this->m_keyBindings.push_back(ButtonBind(key, actionID));
}

uint32_t sge::InputMappingContext::getActionForKey(int key) const
{
    auto it = std::find_if(this->m_keyBindings.begin(), this->m_keyBindings.end(), [key](const ButtonBind &bind) -> bool {
        return bind.button == key;
    });

    if (it != this->m_keyBindings.end())
    {
        return it->actionID;
    }

    // No action/unbounded.
    return 0;
}

const std::vector<sge::ButtonBind>& sge::InputMappingContext::getKeyBinds() const
{
    return this->m_keyBindings;
}

void sge::InputMappingContext::bindMouse(int mouseButton, uint32_t actionID)
{
    this->m_mouseBindings.push_back(sge::ButtonBind(mouseButton, actionID));
}

uint32_t sge::InputMappingContext::getActionForMouse(int mouseButton) const
{
    auto it = std::find_if(this->m_mouseBindings.begin(), this->m_mouseBindings.end(), [mouseButton](const ButtonBind &bind) -> bool {
        return bind.button == mouseButton;
    });

    if (it != this->m_keyBindings.end())
    {
        return it->actionID;
    }

    // No action/unbounded.
    return 0;
}

const std::vector<sge::ButtonBind>& sge::InputMappingContext::getMouseBinds() const 
{
    return this->m_mouseBindings;
}