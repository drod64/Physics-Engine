#include <SGE/systems/physics/BuoyancySystem3.h>

void sge::BuoyancySystem3::update(Registry &registry, CommandBuffer &cmdBuffer, sm::real dt)
{
    auto buoyancy3View = registry.viewAll<sge::CTransform3, sge::CRigidBody3, sge::CBuoyancy3>();

    for (const Entity &e : buoyancy3View)
    {
        // Reads.
        const auto &t3 = buoyancy3View.get<sge::CTransform3>(e);
        const auto &b3 = buoyancy3View.get<sge::CBuoyancy3>(e);

        // Accumulations.
        auto &r3 = buoyancy3View.get<sge::CRigidBody3>(e);
        
        // 1. Get scalar values
        sm::real entityY = t3.position.y;
        sm::real totalHeight = (sm::real)2.0 * b3.maxDepth;
        sm::real bottom = entityY - b3.maxDepth;

        // 2. Calculate submersion ratio based on entity's submersion height
        sm::real rawSubmergedHeight = b3.waterHeight - bottom;
        sm::real clippedSubmergedHeight = std::clamp(rawSubmergedHeight, (sm::real)0.0, totalHeight);
        sm::real submersionRatio = clippedSubmergedHeight / totalHeight;

        // 3. Calculate and add buoyant force.
        sm::Vec3 force((sm::real)0, b3.liquidDensity * b3.volume * submersionRatio, (sm::real)0);
    
        r3.addForce(force);
    }
}

sge::SystemDescriptor sge::BuoyancySystem3::getSystemDescriptor()
{
    sge::SystemDescriptor desc;

    // System phase.
    desc.phase = sge::ExecutionPhase::PhysicsForceGen;

    // System functor.
    desc.functionPtr = &sge::BuoyancySystem3::update;

    // System component reads.
    desc.componentReads.set(sge::ComponentIDCounter::get<sge::CTransform3>());
    desc.componentReads.set(sge::ComponentIDCounter::get<sge::CBuoyancy3>());

    // No system component writes.

    // System component accumulation.
    desc.componentAccumulates.set(sge::ComponentIDCounter::get<sge::CRigidBody3>());

    // No system resource reads.

    // No system resource writes.

    // No system resource accumulation.

    // System name.
    desc.name = "BuoyancySystem3";

    return desc;
}