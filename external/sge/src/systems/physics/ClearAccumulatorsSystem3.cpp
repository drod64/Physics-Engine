#include <SGE/systems/physics/ClearAccumulatorsSystem3.h>
#include <iostream>

void sge::ClearAccumulatorsSystem3::update(sge::Registry &registry, sge::CommandBuffer &, sm::real dt)
{
    // Get pool of CRigidBody.
    auto rigidBodyPool = registry.getOrCreatePool<sge::CRigidBody3>();

    // Early exit if no rigid bodies exist.
    if (!rigidBodyPool) return;

    // Loop through dense pool and reset accumulators.
    for (sge::CRigidBody3 &r3 : rigidBodyPool->getDenseComponents())
    {
        r3.accumulatedForce.x = 0;
        r3.accumulatedForce.y = 0;
        r3.accumulatedForce.z = 0;
    }
}

sge::SystemDescriptor sge::ClearAccumulatorsSystem3::getSystemDescriptor()
{
    sge::SystemDescriptor desc;

    // System phase.
    desc.phase = sge::ExecutionPhase::PhysicsClear;

    // System functor.
    desc.functionPtr = &sge::ClearAccumulatorsSystem3::update;

    // No system component reads.

    // System component writes.
    desc.componentWrites.set(sge::ComponentIDCounter::get<sge::CRigidBody3>());

    // No system component accumulation.

    // No system resource reads.

    // No system resource writes.

    // No system resource accumulation.

    // System name.
    desc.name = "ClearAccumulatorSystem3";

    return desc;
}