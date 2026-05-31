#include <SGE/entities/EntityManager.h>

namespace sge {

EntityManager::EntityManager() :
m_entities(),
m_entitiesToAdd(),
m_entityMap(),
m_totalEntities(0)
{}

void EntityManager::update()
{
    // Entities queued from the last frame will be added to the current frame.
    for (auto e : this->m_entitiesToAdd)
    {
        this->m_entities.push_back(e);
        this->m_entityMap[e->tag()].push_back(e);
    }

    this->m_entitiesToAdd.clear();

    // Remove dead entities from the vector of all entities
    removeDeadEntities(this->m_entities);

    // Remove dead entities from each vector in the entity map.
    for (auto &pair : this->m_entityMap)
    {
        removeDeadEntities(pair.second);
    }
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string &tag)
{
    auto newEntity = std::shared_ptr<Entity>(new Entity(tag, this->m_totalEntities));
    this->m_entitiesToAdd.push_back(newEntity);
    ++this->m_totalEntities;

    return newEntity;
}

void EntityManager::removeDeadEntities(EntityVec &vector)
{
    auto it = std::remove_if(vector.begin(), vector.end(), [](std::shared_ptr<Entity> e) -> bool { return !e->isAlive(); });
    vector.erase(it, vector.end());
}

const EntityVec& EntityManager::getEntities() const
{
    return this->m_entities;
}

const EntityVec* EntityManager::getEntities(const std::string &tag) const
{
    if (this->m_entityMap.find(tag) != this->m_entityMap.end())
    {
        return &this->m_entityMap.at(tag);
    }
    return nullptr;
}


} // namespace sge