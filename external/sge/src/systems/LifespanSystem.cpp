#include <SGE/systems/LifespanSystem.h>

void sge::LifespanSystem::update(sge::Registry &registry, sge::CommandBuffer &cmdBuffer, sm::real dt)
{
    auto *lifespanPool = registry.getOrCreatePool<sge::CLifespan>();

    auto &densePool = lifespanPool->getDenseComponents();

    for (size_t i = 0; i < densePool.size(); ++i)
    {
        densePool[i].remaining -= dt;

        if (densePool[i].remaining <= static_cast<sm::real>(0))
        {
            cmdBuffer.destroyEntityDeferred(lifespanPool->getEntityAt(i));
        }
    }
}

sge::SystemDescriptor sge::LifespanSystem::getSystemDescription()
{
    sge::SystemDescriptor desc;

    // System phase.
    desc.phase = sge::ExecutionPhase::PreUpdate;

    // System functor.
    desc.functionPtr = &sge::LifespanSystem::update;

    // System component accumulation.
    desc.componentAccumulates.set(sge::ComponentIDCounter::get<sge::CLifespan>());

    // System name.
    desc.name = "LifespanSystem";

    return desc;
}