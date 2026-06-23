#include <SGE/systems/physics/AnchorBungeeSystem3.h>

void sge::AnchorBungeeSystem3::update(sge::Registry &registry, sge::CommandBuffer &, sm::real dt)
{
    auto anchorBungee3View = registry.viewAll<sge::CAnchorBungee3, sge::CRigidBody3, sge::CTransform3>();

    for (const Entity &e : anchorBungee3View)
    {
        // Read
        const auto &t3 = anchorBungee3View.get<sge::CTransform3>(e);
        const auto &ab3 = anchorBungee3View.get<sge::CAnchorBungee3>(e);
        
        // Write
        auto &r3 = anchorBungee3View.get<sge::CRigidBody3>(e);

        // Get displacement between both entities.
        sm::Vec3 displacement = t3.position - ab3.anchorPosition;

        // Calculate squared magnitude of displacement vector.
        sm::real sqrLength = displacement.sqrMagnitude() + (sm::real)1e-12;
        sm::real length = real_sqrt(sqrLength);

        sm::real invLength = (sm::real)1.0 / length;
        sm::Vec3 direction = displacement * invLength;

        sm::real stretch = length - ab3.restLength;

        // Only apply pull force if stretch is positive (exceeding rest length).
        sm::real forceMagnitude = (stretch > (sm::real)0.0f) ? -ab3.springConstant * stretch : (sm::real)0.0;

        // Add spring (push or pulling) force to entity.
        r3.addForce(direction * forceMagnitude);
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