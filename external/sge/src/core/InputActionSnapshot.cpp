#include <SGE/core/InputActionSnapshot.h>

const int sge::InputActionSnapshot::INPUT_SIZE = 512;

sge::InputActionSnapshot::InputActionSnapshot()
:
m_mask(sge::InputActionSnapshot::INPUT_SIZE / sge::Bitmask::BIT_SIZE),
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

void sge::InputActionSnapshot::setAction(sge::Action action, bool value)
{
    int i_action = static_cast<int>(action);

    assert(i_action < sge::InputActionSnapshot::INPUT_SIZE);

    int vecIndex = i_action / sge::Bitmask::BIT_SIZE;
    int bitIndex = i_action % sge::Bitmask::BIT_SIZE;

    this->m_mask.at(vecIndex).setBit(bitIndex, value);
}

bool sge::InputActionSnapshot::getActionState(sge::Action action) const
{
    int i_action = static_cast<int>(action);

    assert(i_action < sge::InputActionSnapshot::INPUT_SIZE);

    int vecIndex = i_action / sge::Bitmask::BIT_SIZE;
    int bitIndex = i_action % sge::Bitmask::BIT_SIZE;

    return this->m_mask.at(vecIndex).getBit(bitIndex);
}

void sge::InputActionSnapshot::clear()
{
    // Reset action states.
    std::fill(this->m_mask.begin(), this->m_mask.end(), sge::Bitmask());
    mousePosition = {0, 0};
    mouseDelta = {0, 0};
}