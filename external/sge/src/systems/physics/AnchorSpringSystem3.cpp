#include <SGE/systems/physics/AnchorSpringSystem3.h>

void sge::AnchorSpringSystem3::update(sge::Registry &registry, sge::CommandBuffer &, sm::real dt)
{
    auto anchorSpring3View = registry.viewAll<sge::CTransform3, sge::CRigidBody3, sge::CAnchorSpring3>();
    
    for (const Entity &e : anchorSpring3View)
    {
        // Reads
        const auto &as3 = anchorSpring3View.get<sge::CAnchorSpring3>(e);
        const auto &t3 = anchorSpring3View.get<sge::CTransform3>(e);
        
        // Write
        auto &r3 = anchorSpring3View.get<sge::CRigidBody3>(e);
        
        sm::Vec3 globalAttachPoint = t3.position + t3.orientation.transform(as3.localAttachPoint);
        // Get displacement between both entities.
        sm::Vec3 displacement = globalAttachPoint - as3.anchorPosition;

        // Calculate squared magnitude of displacement vector.
        sm::real sqrLength = displacement.sqrMagnitude() + (sm::real)1e-6f;
        sm::real length = real_sqrt(sqrLength);
        length = std::clamp(length, (sm::real)1e-6f, (sm::real)3.40282e+38f);

        sm::real stretch = length - as3.restLength;

        sm::real forceMagnitude = -as3.springConstant * stretch;

        sm::Vec3 forceVec = displacement * (forceMagnitude / length);

        // Add spring (push or pulling) force to entity.
        sge::RigidBodyUtils::addForceAtGlobalPoint(r3, t3, forceVec, globalAttachPoint);
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