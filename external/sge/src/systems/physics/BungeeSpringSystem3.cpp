#include <SGE/systems/physics/BungeeSpringSystem3.h>
#include <iostream>

void sge::BungeeSpringSystem3::update(sge::Registry &registry, sge::CommandBuffer &cmd, sm::real dt) {
    auto bungeePoolPtr = registry.getOrCreateConstraintPool<sge::BungeeConstraint>();

    const auto bungeePool = bungeePoolPtr->getDenseConstraints();

    for (const sge::BungeeConstraint &constraint : bungeePool)
    {
        if (!registry.isAlive(constraint.entityA) || !registry.isAlive(constraint.entityB)) continue;
        if (!registry.hasComponent<sge::CTransform3>(constraint.entityA) ||
            !registry.hasComponent<sge::CRigidBody3>(constraint.entityA) ||
            !registry.hasComponent<sge::CTransform3>(constraint.entityB) ||
            !registry.hasComponent<sge::CRigidBody3>(constraint.entityB))
        {
            continue;
        }
        // Read
        const auto &aT3 = registry.getComponent<sge::CTransform3>(constraint.entityA);
        const auto &bT3 = registry.getComponent<sge::CTransform3>(constraint.entityB);

        // Accumulate
        auto &aR3 = registry.getComponent<sge::CRigidBody3>(constraint.entityA);
        auto &bR3 = registry.getComponent<sge::CRigidBody3>(constraint.entityB);

        // Get displacement between both entities.
        sm::Vec3 displacement = aT3.position - bT3.position;

        // Calculate squared magnitude of displacement vector.
        sm::real sqrLength = displacement.sqrMagnitude() + (sm::real)1e-6f;
        sm::real length = real_sqrt(sqrLength);
        length = std::clamp(length, (sm::real)1e-6f, (sm::real)3.40282e+38f);

        sm::real stretch = length - constraint.restLength;

        sm::real activeStretch = std::clamp(stretch, (sm::real)0.0, (sm::real)3.40282e+38f);

        sm::real forceMagnitude = -constraint.springConstant * activeStretch;

        sm::Vec3 forceVec = displacement * (forceMagnitude / length);

        // Add spring (pulling) force to entity.
        aR3.addForce(forceVec);
        bR3.addForce(forceVec * -1);
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