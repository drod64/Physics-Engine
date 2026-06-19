#include <SGE/core/registryResources/PlayerInputResource.h>

void sge::PlayerInputResource::updateCurrent(const sge::InputActionSnapshot &current)
{
    this->m_curInput = current;
}

void sge::PlayerInputResource::updatePrevious()
{
    this->m_prevInput = this->m_curInput;
}

bool sge::PlayerInputResource::isActionPressed(sge::Action action) const
{
    return !this->m_prevInput.getActionState(action) && this->m_curInput.getActionState(action);
}

bool sge::PlayerInputResource::isActionHeld(sge::Action action) const
{
    return this->m_prevInput.getActionState(action) && this->m_curInput.getActionState(action);
}

bool sge::PlayerInputResource::isActionReleased(sge::Action action) const
{
    return this->m_prevInput.getActionState(action) && !this->m_curInput.getActionState(action);
}
