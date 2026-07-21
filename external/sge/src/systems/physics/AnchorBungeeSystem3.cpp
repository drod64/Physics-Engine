#include <SGE/systems/physics/AnchorBungeeSystem3.h>

void sge::AnchorBungeeSystem3::update(sge::Registry &registry, sge::CommandBuffer &, sm::real dt)
{
    auto anchorBungee3View = registry.viewAll<sge::CAnchorBungee3, sge::CRigidBody3, sge::CTransform3>();

    for (const Entity &e : anchorBungee3View)
    {
        // Reads
        const auto &ab3 = anchorBungee3View.get<sge::CAnchorBungee3>(e);
        const auto &t3 = anchorBungee3View.get<sge::CTransform3>(e);
        
        // Write
        auto &r3 = anchorBungee3View.get<sge::CRigidBody3>(e);

        // Calculate global attachment point.
        sm::Vec3 globalAttachPoint = t3.position + t3.orientation.transform(ab3.localAttachPoint);
        // Get displacement between both entities.
        sm::Vec3 displacement = globalAttachPoint - ab3.anchorPosition;

        // Calculate squared magnitude of displacement vector.
        sm::real sqrLength = displacement.sqrMagnitude() + (sm::real)1e-6f;
        sm::real length = real_sqrt(sqrLength);
        length = std::clamp(length, (sm::real)1e-6f, (sm::real)3.40282e+38f);

        sm::real stretch = length - ab3.restLength;

        sm::real activeStretch = std::clamp(stretch, (sm::real)0.0, (sm::real)3.40282e+38f);

        sm::real forceMagnitude = -ab3.springConstant * activeStretch;

        sm::Vec3 forceVec = displacement * (forceMagnitude / length);

        // Add spring (pulling) force to entity.
        sge::RigidBodyUtils::addForceAtGlobalPoint(r3, t3, forceVec, globalAttachPoint);
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