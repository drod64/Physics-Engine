#include <SGE/systems/physics/BungeeSpringSystem3.h>

void sge::BungeeSpringSystem3::update(sge::Registry &registry, sge::CommandBuffer &cmd, sm::real dt) {
    auto bungeeSpring3View = registry.viewAll<sge::CTransform3, sge::CRigidBody3, sge::CBungee3>();

    for (const Entity &e : bungeeSpring3View)
    {
        // Read
        const auto &b3 = bungeeSpring3View.get<sge::CBungee3>(e);

        // Only calculate force for one entity. Opposite force will be applied anyways.
        if (b3.targetEntity > e) continue;

        // Early exit if target entity does not have necessary components.
        if (!registry.hasComponent<sge::CTransform3>(b3.targetEntity)) continue;

        // Reads
        const auto &t3 = bungeeSpring3View.get<sge::CTransform3>(e);
        const auto &otherT3 = registry.getComponent<sge::CTransform3>(b3.targetEntity);
        
        // Write
        auto &r3 = bungeeSpring3View.get<sge::CRigidBody3>(e);

        // Get displacement between both entities.
        sm::Vec3 displacement = t3.position - otherT3.position;

        // Calculate squared magnitude of displacement vector.
        sm::real sqrLength = displacement.sqrMagnitude() + (sm::real)1e-6f;
        sm::real length = real_sqrt(sqrLength);
        length = std::clamp(length, (sm::real)1e-6f, (sm::real)3.40282e+38f);

        sm::real stretch = length - b3.restLength;

        sm::real activeStretch = std::clamp(stretch, (sm::real)0.0, (sm::real)3.40282e+38f);

        sm::real forceMagnitude = -b3.springConstant * activeStretch;

        sm::Vec3 forceVec = displacement * (forceMagnitude / length);

        // Add spring (pulling) force to entity.
        r3.addForce(forceVec);
        
        // Apply same force to other object.
        if (registry.hasComponent<sge::CRigidBody3>(b3.targetEntity))
        {
            auto &otherR3 = registry.getComponent<sge::CRigidBody3>(b3.targetEntity);
            otherR3.addForce(forceVec * -1);
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