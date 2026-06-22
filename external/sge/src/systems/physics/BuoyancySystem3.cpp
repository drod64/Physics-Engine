#include <SGE/systems/physics/BuoyancySystem3.h>

void sge::BuoyancySystem3::update(Registry &registry, CommandBuffer &cmdBuffer, sm::real dt)
{
    auto buoyancy3View = registry.viewAll<sge::CTransform3, sge::CRigidBody3, sge::CBuoyancy3>();

    for (const Entity &e : buoyancy3View)
    {
        // Reads.
        sm::real depth = buoyancy3View.get<sge::CTransform3>(e).position.y;
        const auto &b3 = buoyancy3View.get<sge::CBuoyancy3>(e);

        // Writes.
        auto &r3 = buoyancy3View.get<sge::CRigidBody3>(e);
    
        // Get top and bottom bounds of entity.
        sm::real top = depth + b3.maxDepth;
        sm::real bottom = depth - b3.maxDepth;
    
        // (Early exit) Check if it's completely above water.
        if (bottom >= b3.waterHeight) continue;
    
        sm::Vec3 force = {0, 0, 0};
        // Check if entity is completely submerged.
        if (top <= b3.waterHeight)
        {
            force.y = b3.liquidDensity * b3.volume;
        }
        // Otherwise it's partially submerged
        else
        {
            // Get submersion ratio (0 - 1).
            sm::real submersionRatio = (b3.waterHeight - bottom) / (2 * b3.maxDepth);
    
            force.y = b3.liquidDensity * b3.volume * submersionRatio;
        }
    
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