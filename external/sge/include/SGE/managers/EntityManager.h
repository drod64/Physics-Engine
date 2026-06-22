#ifndef SGE_ENTITY_MANAGER_H
#define SGE_ENTITY_MANAGER_H
#include <cstdint>
#include <vector>
#include <queue>
#include <SGE/core/ecs/Entity.h>

namespace sge {
/**
 * Manages the life-cycle of Entity IDs.
 */
class EntityManager {
private:
    static const uint32_t INDEX_MASK;
    static const uint32_t MIN_GENERATION;

    std::vector<uint32_t> m_generations;
    std::queue<uint32_t> m_freeIndices;

    /**
     * Helper function that calculates a unique Entity ID based on given index and generation.
     * @param index the index of the Entity ID
     * @param generation the generation of the Entity ID.
     */
    Entity createEntityID(uint32_t index, uint32_t generation) const;

public:
    /**
     * Default Constructor.
     */
    EntityManager();

    /**
     * Gets the index of an Entity.
     * @param e the Entity to get the index from
     * @return a uint32_t representing the Entity's index
     */
    uint32_t getIndex(sge::Entity e) const;

    /**
     * Gets the generation of an Entity.
     * @param e the Entity to get the generation from
     * @return a uint32_t representing the Entity's generation
     */
    uint32_t getGeneration(sge::Entity e) const;

    /**
     * Creates a brand new Entity.
     * @return the ID of the new Entity
     */
    Entity createEntity();

    /**
     * Removes an Entity from the EntityManager.
     * @param e the Entity to destroy
     */
    void destroyEntity(Entity e);

    /**
     * Checks if an Entity is still alive in the EntityManager.
     * @param e the Entity to check
     * @return true if still alive, false otherwise
     */
    bool isAlive(Entity e) const;
};

} // namespace sge

#endif // SGE_ENTITY_MANAGER_H