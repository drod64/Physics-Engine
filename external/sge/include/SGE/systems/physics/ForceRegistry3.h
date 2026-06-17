#ifndef SGE_FORCE_REGISTRY3_H
#define SGE_FORCE_REGISTRY3_H
#include <memory>
#include <vector>
#include <unordered_map>
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
    
    typedef std::unordered_map<size_t, std::vector<std::unique_ptr<sge::ForceGenerator3>>> Registry3;
    Registry3 m_registrations;

public:
    /**
     * Default constructor.
     */
    ForceRegistry3();

    /**
     * Add an entity and force to the registry.
     * @param e the entity to add
     * @param args the constructor arguments for the force
     * @return a reference to the newly created force
     */
    template <typename T, typename... Args>
    T& add(Entity *e, Args&&... args)
    {   
        // Create force.
        auto unique = std::make_unique<T>(std::forward<Args>(args)...);

        // Store raw address
        T* forcePtr = unique.get();
        
        // Pass ownership to registration data structure.
        this->m_registrations[e->id(), std::move(unique)];

        // Return raw force.
        return *forcePtr;
    }

    /**
     * Remove a specific entity and force from the registry.
     * Useful for removing a specific force from the passed in entity.
     * @param id the ID of the entity to remove
     * @param fg the specific sge::ForceGenerator to remove
     */
    void remove(size_t id, ForceGenerator3 *fg);

    /**
     * Remove a specific entity from the registry.
     * Useful for removing an entity when it's going to be destroyed.
     * @param id the ID of the entity to remove
     */
    void removeEntity(size_t id);

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