#include <SGE/systems/physics/AnchorSpringSystem3.h>

void sge::AnchorSpringSystem3::update(sge::Registry &registry, sge::CommandBuffer &, sm::real dt)
{
    auto anchorSpring3View = registry.viewAll<sge::CTransform3, sge::CRigidBody3, sge::CAnchorSpring3>();
    
    for (const Entity &e : anchorSpring3View)
    {
        // Read
        const auto &t3 = anchorSpring3View.get<sge::CTransform3>(e);
        const auto &as3 = anchorSpring3View.get<sge::CAnchorSpring3>(e);
        
        // Write
        auto &r3 = anchorSpring3View.get<sge::CRigidBody3>(e);

        // Get displacement between both entities.
        sm::Vec3 displacement = t3.position - as3.anchorPosition;

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
        sm::real stretch = length - as3.restLength;
        
        // Early exit if entities are within rest length and spring is slack
        if (real_abs(stretch) < 0.0001) continue;

        // Calculate the final spring force using Hooke's Law while...
        // using the direction vector to know which way it should be applied.
        // F = -k * stretch
        sm::Vec3 springForce = direction * (-as3.springConstant * stretch);

        // Add spring (push or pulling) force to entity.
        r3.addForce(springForce);
    }
}


sge::SystemDescriptor sge::AnchorSpringSystem3::getSystemDescriptor()
{
    SystemDescriptor desc;

    // System phase.
    desc.phase = sge::ExecutionPhase::PhysicsForceGen;

    // System functor.
    desc.functionPtr = &sge::AnchorSpringSystem3::update;

    // System component reads.
    desc.componentReads.set(sge::ComponentIDCounter::get<sge::CTransform3>());
    desc.componentReads.set(sge::ComponentIDCounter::get<sge::CAnchorSpring3>());
    
    // No system component writes.

    // System component accumulation.
    desc.componentAccumulates.set(sge::ComponentIDCounter::get<sge::CRigidBody3>());

    // No system resource reads.

    // No system resource writes.

    // No system resource accumulation.

    // System name.
    desc.name = "AnchorSpringSystem3";

    return desc;
}