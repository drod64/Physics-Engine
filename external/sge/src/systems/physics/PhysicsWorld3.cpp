#include <SGE/systems/physics/PhysicsWorld3.h>

sge::PhysicsWorld3::PhysicsWorld3(unsigned maxContacts, unsigned iterations)
// m_contactResolver(iterations)
{
    // this->m_contacts = new Contact3[this->m_maxContacts];
    this->m_gravity = {0, -9.81, 0};
    this->m_maxContacts = maxContacts;
    this->m_calculateIterations = !iterations;
}

sge::PhysicsWorld3::~PhysicsWorld3()
{
    // delete[] this->m_contacts;
}

void sge::PhysicsWorld3::setGravity(const sm::Vec3 &gravity)
{
    this->m_gravity = gravity;
}

void sge::PhysicsWorld3::startFrame(sge::Registry &registry)
{
    auto pool = registry.getPool<sge::CRigidBody3>();
    auto &components = pool->getDenseComponents();

    for (auto &r3 : components)
    {
        // Clear entity accumulator
        r3.clearAccumulator();
    }
}

unsigned sge::PhysicsWorld3::generateContacts()
{
    // // Modifiable variable to keep track of available memory.
    // unsigned limit = this->m_maxContacts;
    // // Pointer to first contact object in heap-stored array.
    // Contact3 *contactObj = this->m_contacts;

    // // Loop through all contact generators
    // for (auto   it = this->m_contactGens.begin();
    //             it != this->m_contactGens.end();
    //             ++it)
    // {
    //     // Write contacts.
    //     unsigned used = (*it)->addContact(contactObj, limit);
    //     // Keep track of space used so far.
    //     limit -= used;
    //     // Move pointer to next available slot.
    //     contactObj += used;

    //     // If limit is 0 or less, we ran out of contacts to write to. Break.
    //     if (limit <= 0) break;
    // }

    // // Return the number of contacts written.
    // return this->m_maxContacts - limit;
    return 0;
}

void sge::PhysicsWorld3::integrate(sge::Registry &registry, sm::real dt)
{
    auto view = registry.viewAll<sge::CRigidBody3, sge::CTransform3>();

    // Update all entity positions with their velocities and accelerations
    for (Entity e : view)
    {
        sge::CRigidBody3 &r3 = view.get<sge::CRigidBody3>(e);
        sge::CTransform3 &t3 = view.get<sge::CTransform3>(e);

        // Infinite mass object. Continue to next entity.
        if (r3.getMass() <= 0.0f) continue;

        assert(dt > 0.0f);

        // Temporary gravity system until PhysicsSystem is implemented.
        r3.addForce(this->m_gravity * r3.getMass());

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

void sge::PhysicsWorld3::runPhysics(sge::Registry &registry, sge::CommandBuffer &cmdBuffer, sm::real dt)
{   
    // Temporary lifespan system until actual systems are implemented.
    this->sLifespan(registry, cmdBuffer, dt);

    this->startFrame(registry);
    
    // Integrate entities.
    this->integrate(registry, dt);
    
    // Generate contacts.
    unsigned usedContacts = this->generateContacts();
    
    // Resolve contacts.
    // if (this->m_calculateIterations) this->m_contactResolver.setIterations(usedContacts * 2);
    // this->m_contactResolver.resolveContacts(this->m_contacts, usedContacts, dt);
}

void sge::PhysicsWorld3::sLifespan(sge::Registry &registry, sge::CommandBuffer &cmdBuffer, sm::real dt)
{
    auto view = registry.viewAll<sge::CLifespan>();

    for (sge::Entity e : view)
    {
        sge::CLifespan &ls = view.get<sge::CLifespan>(e);

        ls.remaining -= dt;

        if (ls.remaining <= 0)
        {
            cmdBuffer.destroyEntityDeferred(e);
        }
    }
}