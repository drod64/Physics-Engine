#ifndef SGE_WORLD3_H
#define SGE_WORLD3_H
#include <vector>
#include <SM/Precision.h>
#include <SGE/entities/Entity.h>
#include <SGE/systems/physics/ForceRegistry3.h>
#include <SGE/systems/physics/ContactResolver3.h>
#include <SGE/systems/physics/ContactGenerator3.h>

namespace sge {
class World3 {
protected:
    std::vector<Entity*>            m_entities;
    std::vector<ContactGenerator3*> m_contactGens;
    Contact3                        *m_contacts;        // HEAP-MEMORY

    // Helper objects
    ForceRegistry3                  m_forceRegistry;
    ContactResolver3                m_contactResolver;
    
    // Basic data types
    unsigned                        m_maxContacts;
    bool                            m_calculateIterations;

public:
    /**
     * Parameterized Constructor.
     * @param maxContacts the max amount of contacts to be handled per frame
     * @param iterations the number of iterations allowed for contact-resolution. If not explicitly stated, iterations = maxContacts * 2
     */
    World3(unsigned maxContacts, unsigned iterations = 0);

    /**
     * Deconstructor.
     */
    ~World3();

    /**
     * Resets force accumulators of all Entity objects. 
     */
    void startFrame();

    /**
     * Calls all registered contact generators to report any contacts.
     * @return the number of contacts generated
     */
    unsigned generateContacts();

    /**
     * Integrates all the entities in this world by the given delta time.
     * @param dt the delta time between frames
     */
    void integrate(sm::real dt);

    /**
     * Process all physics for the world.
     * @param dt the delta time between frames
     */
    void runPhysics(sm::real dt);
};
}

#endif // SGE_WORLD_H