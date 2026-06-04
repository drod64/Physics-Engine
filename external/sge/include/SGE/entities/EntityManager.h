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

/**
 * Manages the life-cycle and update of entities.
 */
class EntityManager {
private:
    EntityVec m_entities;
    EntityVec m_entitiesToAdd; // a queue for the entites to be added in the next frame
    EntityMap m_entityMap;
    size_t m_totalEntities; // total number of entities spawned in the program.
    void removeDeadEntities(EntityVec &vector);

public:
    /**
     * Default Constructor. 
     */
    EntityManager();

    /**
     * Updates the sge::EntityManager instance.
     * Make sure to put this function call somewhere in your game loop update()
     */
    void update();

    /**
     * Adds an entity to the manager.
     * @param tag the tag of the entity
     * @return a std::shared_ptr<Entity> to the newly added entity
     */
    std::shared_ptr<Entity> addEntity(const std::string &tag);

    /**
     * Retrives ALL entities stored in the manager.
     * @return a const reference to the container with ALL entities
     */
    const EntityVec& getEntities() const;

    /**
     * Retrives entities ONLY with a specific tag.
     * @param tag the tag to search for
     * @return a const reference to the container with entities
     * ONLY with the specific tag
     */
    const EntityVec* getEntities(const std::string &tag) const;
};

} // namespace sge

#endif // SGE_ENTITY_MANAGER_H