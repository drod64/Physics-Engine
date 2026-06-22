#include <SGE/systems/physics/BungeeSpringSystem3.h>

void sge::BungeeSpringSystem3::update(sge::Registry &registry, sge::CommandBuffer &cmd, sm::real dt) {
    auto bungeeSpring3View = registry.viewAll<sge::CTransform3, sge::CRigidBody3, sge::CBungee3>();

    for (const Entity &e : bungeeSpring3View)
    {
        // Reads
        const auto &b3 = bungeeSpring3View.get<sge::CBungee3>(e);

        // 1. Validation gates for the connected entity
        if (!registry.isAlive(b3.other)) continue;
        if (!registry.hasComponent<sge::CTransform3>(b3.other)) continue;
        if (e > b3.other) continue;

        // Reads.
        const auto &t3 = bungeeSpring3View.get<sge::CTransform3>(e);
        const auto &otherT3 = registry.getComponent<sge::CTransform3>(b3.other);

        // Writes.
        auto &r3 = bungeeSpring3View.get<sge::CRigidBody3>(e);

        // 2. Vector math from otherEntity to thisEntity
        sm::Vec3 displacement = t3.position - otherT3.position;
        sm::real sqrLength = displacement.sqrMagnitude();

        // Safe guard for perfect overlap
        if (sqrLength <= 0.00001f)
        {
            r3.addForce(sm::Vec3(0, 1, 0));

            if (registry.hasComponent<sge::CRigidBody3>(b3.other))
            {
                auto &otherR3 = registry.getComponent<sge::CRigidBody3>(b3.other);
                otherR3.addForce(sm::Vec3(0, -1, 0));
            }
            continue;
        }

        sm::real length = real_sqrt(sqrLength);
        sm::real stretch = length - b3.restLength;

        // 3. Early exit if the bungee cord is slack
        if (stretch <= 0.0f) continue;

        // 4. Hooke's Law
        // We want a pulling force. Since 'displacement' points from other -> e,
        // multiplying it by a negative scalar correctly pulls 'e' BACK toward 'other'.
        sm::real forceScalar = (-b3.springConstant * stretch) / length;
        sm::Vec3 springForce = displacement * forceScalar;

        // 5. Apply pulling force to this entity
        r3.addForce(springForce);

        // 6. Apply equal and opposite pulling force to the other entity
        if (registry.hasComponent<sge::CRigidBody3>(b3.other))
        {
            auto &otherR3 = registry.getComponent<sge::CRigidBody3>(b3.other);
            springForce.invert();
            otherR3.addForce(springForce); // Flipped direction pulls 'other' toward 'e'
        }
    }
}


sge::SystemDescriptor sge::BungeeSpringSystem3::getSystemDescriptor()
{
    sge::SystemDescriptor desc;

    // System phase.
    desc.phase = sge::ExecutionPhase::PhysicsForceGen;

    // System functor.
    desc.functionPtr = &sge::BungeeSpringSystem3::update;

    // System component reads.
    desc.componentReads.set(sge::ComponentIDCounter::get<sge::CTransform3>());
    desc.componentReads.set(sge::ComponentIDCounter::get<sge::CBungee3>());

    // No system component writes.

    // System component accumulation.
    desc.componentAccumulates.set(sge::ComponentIDCounter::get<sge::CRigidBody3>());

    // No system resource reads.

    // No system resource writes.

    // No system resource accumulation.

    // System name.
    desc.name = "BungeeSpringSystem3";

    return desc;
}