#include <SGE/systems/physics/DragSystem3.h>

void sge::DragSystem3::update(sge::Registry &registry, sge::CommandBuffer &cmdBuffer, sm::real dt)
{   
    auto drag3View = registry.viewAll<sge::CRigidBody3, sge::CDrag3>();

    for (const Entity &e : drag3View)
    {
        // Reads.
        const auto &d3 = drag3View.get<sge::CDrag3>(e);

        // Writes.
        auto &r3 = drag3View.get<sge::CRigidBody3>(e);
    
        // Calculate squared speed of entity.
        sm::real speedSqr = r3.velocity.sqrMagnitude();
    
        // Early exit if speed is close to 0.
        if (speedSqr < 0.00000001) continue;
        
        // Get real speed.
        sm::real speed = r3.velocity.magnitude();

        // Calculate drag coefficient (using simplified version of (v / speed) * -(k1 * speed + k2 * speed^2))
        sm::real dragScalar = -(d3.k1 + d3.k2 * speed);
    
        // Multiply velocity by negative drag coefficient (will produce drag in the opposite way it is heading).
        r3.addForce(r3.velocity * dragScalar);
    }
}

sge::SystemDescriptor sge::DragSystem3::getSystemDescriptor()
{
    sge::SystemDescriptor desc;

    // System phase.
    desc.phase = sge::ExecutionPhase::PhysicsForceGen;

    // System functor.
    desc.functionPtr = &sge::DragSystem3::update;

    // System component reads.
    desc.componentReads.set(sge::ComponentIDCounter::get<sge::CDrag3>());

    // No system component writes.

    // System component accumulation.
    desc.componentAccumulates.set(sge::ComponentIDCounter::get<sge::CRigidBody3>());

    // No system resource reads.

    // No system resource writes.

    // No system resource accumulation.

    // System name.
    desc.name = "DragSystem3";

    return desc;
}