#include <SGE/systems/physics/IntegrationSystem3.h>

void sge::IntegrationSystem3::update(sge::Registry &registry, sge::CommandBuffer &, sm::real dt)
{
    auto view = registry.viewAll<sge::CRigidBody3, sge::CTransform3>();

    // Update all entity positions with their velocities and accelerations
    for (const Entity &e : view)
    {
        sge::CRigidBody3 &r3 = view.get<sge::CRigidBody3>(e);
        sge::CTransform3 &t3 = view.get<sge::CTransform3>(e);

        // Static entity. Continue to next entity.
        if (r3.isStatic()) continue;

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

sge::SystemDescriptor sge::IntegrationSystem3::getSystemDescriptor()
{
    sge::SystemDescriptor desc;

    // System phase.
    desc.phase = sge::ExecutionPhase::PhysicsIntegrate;

    // System functor.
    desc.functionPtr = &sge::IntegrationSystem3::update;

    // No system component reads.

    // No system component writes.

    // System component accumulation.
    desc.componentAccumulates.set(sge::ComponentIDCounter::get<sge::CRigidBody3>());
    desc.componentAccumulates.set(sge::ComponentIDCounter::get<sge::CTransform3>());

    // No system resource reads.

    // No system resource writes.

    // No system resource accumulation.

    // System name.
    desc.name = "IntegrationSystem3";

    return desc;
}