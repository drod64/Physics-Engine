#include <SGE/core/InputSnapshot.h>

const int sge::InputSnapshot::INPUT_SIZE = 512;

sge::InputSnapshot::InputSnapshot()
:
m_keyButtons(INPUT_SIZE / sge::Bitmask::BIT_SIZE, sge::Bitmask()),
m_mouseButtons(INPUT_SIZE / sge::Bitmask::BIT_SIZE, sge::Bitmask()),
mouseDelta(0,0),
mousePosition(0,0)
{}

sge::InputSnapshot::InputSnapshot(const sge::InputSnapshot &otherInput)
:
sge::InputSnapshot()
{
    *this = otherInput;
}

sge::InputSnapshot& sge::InputSnapshot::operator = (const sge::InputSnapshot &otherInput)
{
    if (this != &otherInput)
    {
        this->m_keyButtons = otherInput.m_keyButtons;
        this->m_mouseButtons = otherInput.m_mouseButtons;
        this->mouseDelta = otherInput.mouseDelta;
        this->mousePosition = otherInput.mousePosition;
    }

    return *this;
}

void sge::InputSnapshot::setKeyButton(int keyButton, bool value)
{
    assert(keyButton < sge::InputSnapshot::INPUT_SIZE);

    int vecIndex = keyButton / sge::Bitmask::BIT_SIZE;
    int bitIndex = keyButton % sge::Bitmask::BIT_SIZE;

    this->m_keyButtons.at(vecIndex).setBit(bitIndex, value);
}

void sge::InputSnapshot::setMouseButton(int mouseButton, bool value)
{
    assert(mouseButton < sge::InputSnapshot::INPUT_SIZE);

    int vecIndex = mouseButton / sge::Bitmask::BIT_SIZE;
    int bitIndex = mouseButton % sge::Bitmask::BIT_SIZE;

    this->m_mouseButtons.at(vecIndex).setBit(bitIndex, value);
}

bool sge::InputSnapshot::getKeyButtonState(int keyButton) const
{
    assert(keyButton < sge::InputSnapshot::INPUT_SIZE);

    int vecIndex = keyButton / sge::Bitmask::BIT_SIZE;
    int bitIndex = keyButton % sge::Bitmask::BIT_SIZE;

    return this->m_keyButtons.at(vecIndex).getBit(bitIndex);
}

bool sge::InputSnapshot::getMouseButtonState(int mouseButton) const
{
    assert(mouseButton < sge::InputSnapshot::INPUT_SIZE);

    int vecIndex = mouseButton / sge::Bitmask::BIT_SIZE;
    int bitIndex = mouseButton % sge::Bitmask::BIT_SIZE;

    return this->m_mouseButtons.at(vecIndex).getBit(bitIndex);
}