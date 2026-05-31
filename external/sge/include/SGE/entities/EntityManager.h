#ifndef SGE_ENTITY_MANAGER_H
#define SGE_ENTITY_MANAGER_H
#include <algorithm>
#include <memory>
#include <vector>
#include <unordered_map>
#include <SGE/entities/Entity.h>

namespace sge {

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::unordered_map<std::string, EntityVec> EntityMap;

class EntityManager {
private:
    EntityVec m_entities;
    EntityVec m_entitiesToAdd; // a queue for the entites to be added in the next frame
    EntityMap m_entityMap;
    size_t m_totalEntities; // total number of entities spawned in the program.
    void removeDeadEntities(EntityVec &vector);

public:
    EntityManager();
    void update();
    std::shared_ptr<Entity> addEntity(const std::string &tag);
    const EntityVec& getEntities() const;
    const EntityVec* getEntities(const std::string &tag) const;
};

} // namespace sge

#endif // SGE_ENTITY_MANAGER_H