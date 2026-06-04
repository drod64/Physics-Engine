#ifndef SGE_FORCE_REGISTRY3_H
#define SGE_FORCE_REGISTRY3_H
#include <algorithm>
#include <SGE/entities/Entity.h>
#include <SGE/systems/physics/ForceGenerator3.h>

namespace sge {
/**
 * Stores entities with an attached sge::ForceGenerator to them.
 * Supports addition, removal, and update of these entities with
 * their specific forces.
 */
class ForceRegistry3 {
protected:
    struct ForceRegistration3 {
        Entity *e;
        ForceGenerator3 *fg;
    };

    typedef std::vector<ForceRegistration3> Registry3;
    Registry3 m_registrations;

public:
    /**
     * Default constructor.
     */
    ForceRegistry3();

    /**
     * Add an entity and force to the registry.
     * @param e the entity to add
     * @param fg the sge::ForceGenerator that should be used in updates
     */
    void add(Entity *e, ForceGenerator3 *fg);

    /**
     * Remove a specific entity and force from the registry.
     * Useful for removing a specific force from the passed in entity.
     * @param e the entity to remove
     * @param fg the specific sge::ForceGenerator to remove
     */
    void remove(Entity *e, ForceGenerator3 *fg);

    /**
     * Remove a specific entity from the registry.
     * Useful for removing an entity when it's going to be destroyed.
     * @param e the entity to remove
     */
    void removeEntity(Entity *e);

    /**
     * Remove a specific force from all entities in the registry.
     * Useful for removing an environmental force such as wind, gravity, etc.
     * @param fg the specific sge::ForceGenerator to remove
     */
    void removeForce(ForceGenerator3 *fg);

    /**
     * Clears all entities and linked sge::ForceGenerators from the registry.
     */
    void clear();

    /**
     * Updates ALL entities with their linked sge::ForceGenerators.
     * @param dt the delta time between frames
     */
    void updateForces(sm::real dt);
};
}

#endif // SGE_FORCE_REGISTRY3_H