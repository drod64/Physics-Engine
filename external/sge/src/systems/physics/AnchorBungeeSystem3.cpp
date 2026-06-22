#include <SGE/systems/physics/AnchorBungeeSystem3.h>

void sge::AnchorBungeeSystem3::update(sge::Registry &registry, sge::CommandBuffer &, sm::real dt)
{
    auto anchorBungee3View = registry.viewAll<sge::CAnchorBungee3, sge::CRigidBody3, sge::CTransform3>();

    for (const Entity &e : anchorBungee3View)
    {
        // Reads.
        const auto &t3 = anchorBungee3View.get<sge::CTransform3>(e);
        const auto &b3 = anchorBungee3View.get<sge::CAnchorBungee3>(e);

        // Writes.
        auto &r3 = anchorBungee3View.get<sge::CRigidBody3>(e);
    
        // Get displacement between both entities.
        sm::Vec3 displacement = t3.position - b3.anchorPosition;
    
        // Calculate squared magnitude of displacement vector.
        sm::real sqrLength = displacement.sqrMagnitude();
    
        // Early exit if the squared magnitude is 0.
        if (sqrLength == 0) continue;
    
        // Cache real length of displacement vector.
        sm::real length = real_sqrt(sqrLength);
        
        // Normalize the displacement vector to get the direction it is currently stretching.
        sm::Vec3 direction = displacement * ((sm::real)1 / length);
    
        // Calculate the stretch
        sm::real stretch = length - b3.restLength;
        
        // Early exit if entities are within rest length and spring is slack
        if (stretch <= 0) continue;
    
        // Calculate the final spring force using Hooke's Law while...
        // using the direction vector to know which way it should be applied.
        // F = -k * stretch
        sm::Vec3 springForce = direction * -b3.springConstant * stretch;
    
        // Add spring (pulling) force to entity.
        r3.addForce(springForce);
    }
}

sge::SystemDescriptor sge::AnchorBungeeSystem3::getSystemDescriptor()
{
    SystemDescriptor desc;

    // System phase.
    desc.phase = sge::ExecutionPhase::PhysicsForceGen;

    // System functor.
    desc.functionPtr = &sge::AnchorBungeeSystem3::update;

    // System component reads.
    desc.componentReads.set(sge::ComponentIDCounter::get<sge::CTransform3>());
    desc.componentReads.set(sge::ComponentIDCounter::get<sge::CAnchorBungee3>());
    
    // No system component writes.

    // System component accumulation.
    desc.componentAccumulates.set(sge::ComponentIDCounter::get<sge::CRigidBody3>());

    // No system resource reads.

    // No system resource writes.

    // No system resource accumulation.

    // System name.
    desc.name = "AnchorBungeeSystem3";

    return desc;
}