#include <SGE/systems/physics/World3.h>

sge::World3::World3(unsigned maxContacts, unsigned iterations = 0) :
m_forceRegistry(),
m_contactResolver(iterations),
m_maxContacts(maxContacts)
{
    this->m_contacts = new Contact3[this->m_maxContacts];
    this->m_calculateIterations = !iterations;
}

sge::World3::~World3()
{
    delete[] this->m_contacts;
}

void sge::World3::startFrame()
{
    for (auto   it = this->m_entities.begin();
                it != this->m_entities.end();
                ++it)
    {
        // Get component of current entity.
        auto &r3 = (*it)->getComponent<sge::CRigidBody3>();

        // Clear entity accumulator
        r3.clearAccumulator();
    }
}

unsigned sge::World3::generateContacts()
{
    // Modifiable variable to keep track of available memory.
    unsigned limit = this->m_maxContacts;
    // Pointer to first contact object in heap-stored array.
    Contact3 *contactObj = this->m_contacts;

    // Loop through all contact generators
    for (auto   it = this->m_contactGens.begin();
                it != this->m_contactGens.end();
                ++it)
    {
        // Write contacts.
        unsigned used = (*it)->addContact(contactObj, limit);
        // Keep track of space used so far.
        limit -= used;
        // Move pointer to next available slot.
        contactObj += used;

        // If limit is 0 or less, we ran out of contacts to write to. Break.
        if (limit <= 0) break;
    }

    // Return the number of contacts written.
    return this->m_maxContacts - limit;
}

void sge::World3::integrate(sm::real dt)
{
    // Update all entity positions with their velocities and accelerations
    for (auto it = this->m_entities.begin(); it != this->m_entities.end(); ++it)
    {
        Entity *e = (*it);

        if (e->hasComponent<sge::CRigidBody3>() &&
            e->hasComponent<sge::CTransform3>())
        {
            sge::CRigidBody3 &r3 = e->getComponent<sge::CRigidBody3>();
            sge::CTransform3 &t3 = e->getComponent<sge::CTransform3>();

            // Infinite mass object. Continue to next entity.
            if (r3.getMass() <= 0.0f) continue;

            assert(dt > 0.0f);

            // Calculate acceleration (dependending on acting forces) and integrate velocity
            r3.velocity.addScaledVector(r3.accumulatedForce * r3.getInverseMass(), dt);
            
            // Numerical rest check
            if (r3.velocity.sqrMagnitude() < 0.0001)
            {
                r3.velocity = {0, 0, 0};
            }
            
            // Integrate position
            t3.position.addScaledVector(r3.velocity, dt);
        }
    }
}

void sge::World3::runPhysics(sm::real dt)
{   
    // Update forces.
    this->m_forceRegistry.updateForces(dt);

    // Integrate entities.
    this->integrate(dt);

    // Generate contacts.
    unsigned usedContacts = this->generateContacts();

    // Resolve contacts.
    if (this->m_calculateIterations) this->m_contactResolver.setIterations(usedContacts * 2);
    this->m_contactResolver.resolveContacts(this->m_contacts, usedContacts, dt);
}