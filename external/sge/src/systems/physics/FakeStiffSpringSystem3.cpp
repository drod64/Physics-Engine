#include <SGE/systems/physics/FakeStiffSpringSystem3.h>

void sge::FakeStiffSpringSystem3::update(sge::Registry &registry, sge::CommandBuffer &, sm::real dt)
{
    auto fakeStiffView = registry.viewAll<sge::CTransform3, sge::CRigidBody3, sge::CFakeStiffSpring3>();

    for (const Entity &e : fakeStiffView)
    {
        // Reads.
        const auto &t3 = fakeStiffView.get<sge::CTransform3>(e);
        const auto &fss3 = fakeStiffView.get<sge::CFakeStiffSpring3>(e);

        // Writes.
        auto &r3 = fakeStiffView.get<sge::CRigidBody3>(e);
    
        // Early exit if entity is static.
        if (r3.isStatic()) continue;
    
        // Get displacement vector
        sm::Vec3 displacement = t3.position - fss3.anchorPosition;
    
        // Calculate the constants and check whether they are in bounds.
        sm::real gamma = 0.5f * real_sqrt(4 * fss3.springConstant - fss3.damping * fss3.damping);
    
        if (gamma == 0.0f) continue;
    
        sm::Vec3 c = displacement * (fss3.damping / (2.0f * gamma)) +
                    r3.velocity * (1.0f / gamma);
    
        // Calculate the target position.
        sm::Vec3 target = displacement * real_cos(gamma * dt) +
                        c * real_sin(gamma * dt);
    
        target *= real_exp(-0.5f * dt * fss3.damping);
    
        // Calculate the resulting acceleration.
        sm::Vec3 accel = (target - displacement) * (1.0f / dt * dt) -
                        r3.velocity * dt;
    
        r3.addForce(accel * r3.getMass());
    }
}

sge::SystemDescriptor sge::FakeStiffSpringSystem3::getSystemDescriptor()
{
    sge::SystemDescriptor desc;

    // System phase.
    desc.phase = sge::ExecutionPhase::PhysicsForceGen;

    // System functor.
    desc.functionPtr = &sge::FakeStiffSpringSystem3::update;

    // System component reads.
    desc.componentReads.set(sge::ComponentIDCounter::get<sge::CFakeStiffSpring3>());
    desc.componentReads.set(sge::ComponentIDCounter::get<sge::CTransform3>());

    // No system component writes.

    // System component accumulation.
    desc.componentAccumulates.set(sge::ComponentIDCounter::get<sge::CRigidBody3>());

    // No system resource reads.

    // No system resource writes.

    // No system resource accumulation.

    // System name.
    desc.name = "FakeStiffSpringSystem3";

    return desc;
}