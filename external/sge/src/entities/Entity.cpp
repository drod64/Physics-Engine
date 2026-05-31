#include <SGE/entities/Entity.h>

namespace sge {

Entity::Entity() :
Entity("Default", 0)
{}

Entity::Entity(const std::string &tag, size_t id) :
m_TAG(tag),
m_ID(id),
m_isAlive(true)
{}

void Entity::destroy()
{
    this->m_isAlive = false;
}

bool Entity::isAlive() const
{
    return this->m_isAlive;
}

size_t Entity::id() const
{
    return this->m_ID;
}

const std::string& Entity::tag() const
{
    return this->m_TAG;
}

}
