#include <SGE/systems/physics/AnchorBungeeSystem3.h>

void sge::AnchorBungeeSystem3::update(sge::Registry &registry, sge::CommandBuffer &, sm::real dt)
{
    auto anchorBungee3View = registry.viewAll<sge::CAnchorBungee3, sge::CRigidBody3, sge::CTransform3>();

    for (const Entity &e : anchorBungee3View)
    {
        // Write only.
        auto &r3 = anchorBungee3View.get<sge::CRigidBody3>(e);

        // Read only.
        const auto &t3 = anchorBungee3View.get<sge::CTransform3>(e);
        const auto &ab3 = anchorBungee3View.get<sge::CAnchorBungee3>(e);

        // Get displacement between both entities.
        sm::Vec3 displacement = t3.position - ab3.anchorPosition;

        // Calculate squared magnitude of displacement vector.
        sm::real length = displacement.magnitude();

        sm::Vec3 direction;

        if (length <= 0.00001)
        {
            // Fallback if entity is directly on top of anchor point.
            direction = {0, 1, 0};
            // Clamp length to 0.
            length = (sm::real)0;

            // Apply nudge to entity.
            r3.addForce(direction * (sm::real)1);
            continue;
        }

        direction = displacement * ((sm::real)1 / length);
        // Calculate the stretch
        sm::real stretch = length - ab3.restLength;
        
        // Early exit if entities are within rest length and spring is slack
        if (stretch <= 0) continue;

        // Calculate the final spring force using Hooke's Law while...
        // using the direction vector to know which way it should be applied.
        // F = -k * stretch
        sm::Vec3 springForce = direction * (-ab3.springConstant * stretch);

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
    
    // System component writes.
    // desc.componentWrites.set(sge::ComponentIDCounter::get<sge::CRigidBody3>());

    // No system resource reads.

    // No system resource writes.

    // System name.
    desc.name = "AnchorBungeeSystem3";

    return desc;
}