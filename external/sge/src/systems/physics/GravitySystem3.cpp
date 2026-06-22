#include <SGE/systems/physics/GravitySystem3.h>

void sge::GravitySystem3::update(sge::Registry &registry, sge::CommandBuffer &, sm::real dt)
{
    auto gravity3View = registry.viewAll<sge::CGravity3, sge::CRigidBody3>();

    for (const Entity &e : gravity3View)
    {
        // Reads.
        const auto &g3 = gravity3View.get<sge::CGravity3>(e);

        // Writes.
        auto &r3 = gravity3View.get<sge::CRigidBody3>(e);
        
        // Ignore immovable entities
        if (r3.isStatic()) continue;

        // Update movable entities with gravitational force
        r3.addForce(g3.gravity * r3.getMass());
    }
}

sge::SystemDescriptor sge::GravitySystem3::getSystemDescriptor()
{
    sge::SystemDescriptor desc;

    // System phase.
    desc.phase = sge::ExecutionPhase::PhysicsForceGen;

    // System functor.
    desc.functionPtr = &sge::GravitySystem3::update;

    // System component reads.
    desc.componentReads.set(sge::ComponentIDCounter::get<sge::CGravity3>());

    // No system component writes.

    // System component accumulation.
    desc.componentAccumulates.set(sge::ComponentIDCounter::get<sge::CRigidBody3>());

    // No system resource reads.

    // No system resource writes.

    // No system resource accumulation.

    // System name.
    desc.name = "GravitySystem3";

    return desc;
}