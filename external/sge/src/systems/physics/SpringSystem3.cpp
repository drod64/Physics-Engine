#include <SGE/systems/physics/SpringSystem3.h>

void sge::SpringSystem3::update(sge::Registry &registry, sge::CommandBuffer &, sm::real dt)
{
    auto spring3View = registry.viewAll<sge::CSpring3, sge::CTransform3, sge::CRigidBody3>();

    for (const Entity &e : spring3View)
    {
        // Read
        const auto &t3 = spring3View.get<sge::CTransform3>(e);
        const auto &s3 = spring3View.get<sge::CSpring3>(e);

        // Early exit if other entity does not exist.
        if (!registry.isAlive(s3.other)) continue;

        // Early exit if the other entity does not have a transform component.
        if (!registry.hasComponent<sge::CTransform3>(s3.other)) continue;
        
        // Get the other Entity's transform component.
        const auto &otherT3 = registry.getComponent<sge::CTransform3>(s3.other);

        // Write
        auto &r3 = spring3View.get<sge::CRigidBody3>(e);

        // Get displacement between both entities.
        sm::Vec3 displacement = t3.position - otherT3.position;

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
        sm::real stretch = length - s3.restLength;
        
        // Early exit if entities are within rest length and spring is slack
        if (real_abs(stretch) < 0.0001) continue;

        // Calculate the final spring force using Hooke's Law while...
        // using the direction vector to know which way it should be applied.
        // F = -k * stretch
        sm::Vec3 springForce = direction * (-s3.springConstant * stretch);

        // Add spring (push or pulling) force to entity.
        r3.addForce(springForce);
    }
}

sge::SystemDescriptor sge::SpringSystem3::getSystemDescriptor()
{
    sge::SystemDescriptor desc;

    // System phase.
    desc.phase = sge::ExecutionPhase::PhysicsForceGen;

    // System functor.
    desc.functionPtr = &sge::SpringSystem3::update;

    // System component reads.
    desc.componentReads.set(sge::ComponentIDCounter::get<sge::CTransform3>());
    desc.componentReads.set(sge::ComponentIDCounter::get<sge::CSpring3>());

    // No system component writes.

    // System component accumulation.
    desc.componentAccumulates.set(sge::ComponentIDCounter::get<sge::CRigidBody3>());

    // No system resource reads.

    // No system resource writes.

    // No system resource accumulation.

    // System name.
    desc.name = "SpringSystem3";

    return desc;
}