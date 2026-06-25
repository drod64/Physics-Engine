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
        
        // 1. Get scalar values.
        sm::real entityY = t3.position.y;
        sm::real absTotalHeight = std::abs((sm::real)2.0 * b3.maxDepth);
        absTotalHeight = std::clamp(absTotalHeight, (sm::real)1e-6f, (sm::real)3.40282e+38f);
        
        sm::real bottom = entityY - b3.maxDepth;

        // 2. Calculate submersion ratio based on entity's submersion height.
        sm::real rawSubmergedHeight = b3.waterHeight - bottom;
        // Fixed clamp() from crashing to due hi < lo.
        sm::real clippedSubmergedHeight = std::clamp(rawSubmergedHeight, (sm::real)0.0, absTotalHeight);
        sm::real submersionRatio = clippedSubmergedHeight / absTotalHeight;

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