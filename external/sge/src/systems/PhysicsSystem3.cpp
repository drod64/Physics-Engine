#include <SGE/systems/PhysicsSystem3.h>

void sge::PhysicsSystem3::update(sge::Registry &registry, sge::CommandBuffer &, sm::real dt)
{
    // GRAVITY FORCE
    auto gravity3View = registry.viewAll<sge::CRigidBody3, sge::CGravity3>();
    for (const Entity &e : gravity3View)
    {   
        auto &r3 = gravity3View.get<sge::CRigidBody3>(e);
        const auto &g3 = gravity3View.get<sge::CGravity3>(e);
        
        r3.addForce(g3.gravity * r3.getMass());
    }
    
    // INTEGRATE ALL FORCES FOR THIS FRAME
    integrate(registry, dt);
    
    // CLEAR ALL ACCUMULATE FORCES FROM LAST FRAME
    startFrame(registry);
}

sge::SystemDescriptor sge::PhysicsSystem3::getSystemDescription()
{
    sge::SystemDescriptor desc;

    // System functor.
    desc.functionPtr = &sge::PhysicsSystem3::update;

    // System component reads.
    desc.componentReads.set(sge::ComponentIDCounter::get<sge::CTransform3>());
    desc.componentReads.set(sge::ComponentIDCounter::get<sge::CGravity3>());

    // System component writes.
    desc.componentWrites.set(sge::ComponentIDCounter::get<sge::CRigidBody3>());

    // No resource reads/writes.
    
    // System name.
    desc.name = "PhysicsSystem3";

    return desc;
}

void sge::PhysicsSystem3::startFrame(Registry &registry)
{
    auto pool = registry.getOrCreatePool<sge::CRigidBody3>();
    auto &components = pool->getDenseComponents();
    
    for (auto &r3 : components)
    {
        // Clear entity accumulator
        r3.clearAccumulator();
    }
}

void sge::PhysicsSystem3::integrate(Registry &registry, sm::real dt)
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

        // Calculate acceleration (dependending on acting forces) and integrate velocity
        r3.velocity.addScaledVector(r3.accumulatedForce * r3.getInverseMass(), dt);
        
        // Numerical rest check
        if (r3.velocity.sqrMagnitude() < 0.00001)
        {
            r3.velocity = {0, 0, 0};
        }
        
        // Integrate position
        t3.position.addScaledVector(r3.velocity, dt);
    }
}