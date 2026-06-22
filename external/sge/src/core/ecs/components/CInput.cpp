#include <SGE/core/ecs/components/CInput.h>

namespace sge {

CInput::CInput()
{
    this->m_input_state.push_back(sge::Bitmask());
}

bool sge::CInput::getInputState(int action)
{
    std::pair<int, int> indexes = this->getIndexes(action);

    return this->m_input_state.at(indexes.first).getBit(indexes.second);
}

void sge::CInput::setInputState(int action, bool value)
{
    std::pair<int, int> indexes = this->getIndexes(action);

    this->m_input_state.at(indexes.first).setBit(indexes.second, value);
}

std::pair<int, int> sge::CInput::getIndexes(int action)
{
    int vecIndex = action / 64;
    int bitIndex = action % 64;

    // If short on Bitmasks, add more.
    if (this->m_input_state.size() < vecIndex + 1)
    {
        this->m_input_state.insert(this->m_input_state.end(), vecIndex + 1 - this->m_input_state.size(), Bitmask());
    }

    return {vecIndex, bitIndex};
}

} // namespace sge