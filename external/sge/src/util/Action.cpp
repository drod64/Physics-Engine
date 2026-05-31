#include <SGE/util/Action.h>

sge::Action::Action()
{
    this->m_id = -1; // NULL action ID
    this->m_type = sge::ActionType::NONE; // NULL action type
}

sge::Action::Action(sge::ActionID id, sge::ActionType type)
:
m_id(id), m_type(type)
{}

sge::ActionID sge::Action::getID() const
{
    return this->m_id;
}

sge::ActionType sge::Action::getType() const
{
    return this->m_type;
}

void sge::Action::setID(sge::ActionID id)
{
    this->m_id = id;
}

void sge::Action::setType(sge::ActionType type)
{
    this->m_type = type;
}