#ifndef SGE_PHYSICS_WORLD3_H
#define SGE_PHYSICS_WORLD3_H
#include <vector>
#include <SM/Vec3.h>
#include <SGE/components/CRigidBody3.h>
#include <SGE/components/CTransform3.h>
#include <SGE/components/CLifespan.h>
#include <SGE/core/View.h>
#include <SGE/core/CommandBuffer.h>
#include <SGE/systems/PhysicsSystem3.h>

namespace sge {
class PhysicsWorld3 {
private:
    sm::Vec3                        m_gravity;
    // std::vector<ContactGenerator3*> m_contactGens;
    // ContactResolver3                m_contactResolver;
    // Contact3                        *m_contacts;        // HEAP-MEMORY

    unsigned                        m_maxContacts;
    bool                            m_calculateIterations;

    /**
     * Calls all registered contact generators to report any contacts.
     * @return the number of contacts generated
     */
    unsigned generateContacts();

    /**
     * Integrates all the entities in this world by the given delta time.
     * @param dt the delta time between frames
     */
    void integrate(Registry &registry, sm::real dt);

    void sLifespan(Registry &registry, CommandBuffer &cmdBuffer, sm::real dt);

public:

    PhysicsWorld3(unsigned maxContacts, unsigned iterations = 0);

    ~PhysicsWorld3();

    /**
     * Sets the gravity of the World.
     * @param gravity the gravity force that will apply to all non-static entities
     */
    void setGravity(const sm::Vec3 &gravity);

    /**
     * Resets force accumulators of all Entity objects. 
     */
    void startFrame(Registry &registry);

    /**
     * Process all physics for the world.
     * @param dt the delta time between frames
     */
    void runPhysics(Registry &registry, CommandBuffer &cmdBuffer, sm::real dt);
};
}

#endif // SGE_PHYSICS_WORLD3_H