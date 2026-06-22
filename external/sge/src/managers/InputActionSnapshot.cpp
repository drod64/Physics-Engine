#include <SGE/managers/InputActionSnapshot.h>

sge::InputActionSnapshot::InputActionSnapshot()
:
m_mask(),
mouseDelta(0,0),
mousePosition(0,0)
{}

sge::InputActionSnapshot::InputActionSnapshot(const sge::InputActionSnapshot &otherInput)
:
sge::InputActionSnapshot()
{
    *this = otherInput;
}

sge::InputActionSnapshot& sge::InputActionSnapshot::operator = (const sge::InputActionSnapshot &otherInput)
{
    if (this != &otherInput)
    {
        this->m_mask = otherInput.m_mask;
        this->mouseDelta = otherInput.mouseDelta;
        this->mousePosition = otherInput.mousePosition;
    }

    return *this;
}

void sge::InputActionSnapshot::setAction(uint32_t actionID, bool value)
{
    this->m_mask.set(actionID, value);
}

bool sge::InputActionSnapshot::getActionState(uint32_t actionID) const
{
    return this->m_mask.test(actionID);
}

void sge::InputActionSnapshot::reset()
{
    // Reset action states.
    this->m_mask.reset();
    mousePosition = {0, 0};
    mouseDelta = {0, 0};
}