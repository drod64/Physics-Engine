#include <SGE/systems/PhysicsSystem3.h>

void sge::PhysicsSystem3::update(sge::Registry &registry, sge::CommandBuffer &, sm::real dt)
{
    // CLEAR ALL ACCUMULATE FORCES FROM LAST FRAME
    startFrame(registry);

    // GRAVITY FORCE
    auto gravity3View = registry.viewAll<sge::CRigidBody3, sge::CGravity3>();
    for (const Entity &e : gravity3View)
    {
        auto &r3 = gravity3View.get<sge::CRigidBody3>(e);
        const auto &g3 = gravity3View.get<sge::CGravity3>(e);
        
        r3.addForce(g3.gravity * r3.getMass());
    }

    // ANCHOR BUNGEE FORCE
    auto anchorBungee3View = registry.viewAll<sge::CRigidBody3, sge::CTransform3, sge::CAnchorBungee3>();

    for (const Entity &e : anchorBungee3View)
    {
        // Retrieve necessary components from entity.
        auto &r3 = anchorBungee3View.get<sge::CRigidBody3>(e);
        const auto &t3 = anchorBungee3View.get<sge::CTransform3>(e);
        const auto &ab3 = anchorBungee3View.get<sge::CAnchorBungee3>(e);

        // Get displacement between both entities.
        sm::Vec3 displacement = t3.position - ab3.anchorPosition;

        // Calculate squared magnitude of displacement vector.
        sm::real length = displacement.magnitude();

        sm::Vec3 direction;

        if (length > 0.0001)
        {
            // Get normalized vector of displacement (which is the direction it is stretched/compressed).
            direction = displacement * ((sm::real)1 / length);
        }
        else
        {
            // Fallback if entity is directly on top of anchor point.
            direction = {0, 1, 0};
            // Clamp length to 0.
            length = (sm::real)0;
        }

        // Calculate the stretch
        sm::real stretch = length - ab3.restLength;
        
        // Early exit if entities are within rest length and spring is slack
        if (stretch <= 0 && length > 0) break;

        // Calculate the final spring force using Hooke's Law while...
        // using the direction vector to know which way it should be applied.
        // F = -k * stretch
        sm::Vec3 springForce = direction * -ab3.springConstant * stretch;

        // Add spring (pulling) force to entity.
        r3.addForce(springForce);

        // Give a little nudge to entities stuck directly on anchor point.
        if (length == 0)
        {
            r3.addForce(direction * (sm::real)100);
        }
    }

    // INTEGRATE ALL FORCES FOR THIS FRAME
    integrate(registry, dt);
}

sge::SystemDescriptor sge::PhysicsSystem3::getSystemDescription()
{
    sge::SystemDescriptor desc;

    // System functor.
    desc.functionPtr = &sge::PhysicsSystem3::update;

    // System component reads.
    desc.componentReads.set(sge::ComponentIDCounter::get<sge::CTransform3>());
    desc.componentReads.set(sge::ComponentIDCounter::get<sge::CRigidBody3>());
    desc.componentReads.set(sge::ComponentIDCounter::get<sge::CGravity3>());
    desc.componentReads.set(sge::ComponentIDCounter::get<sge::CAnchorBungee3>());

    // System component writes.
    desc.componentWrites.set(sge::ComponentIDCounter::get<sge::CRigidBody3>());

    // No resource reads/writes.
    
    // System name.
    desc.name = "PhysicsSystem3";

    return desc;
}

void sge::PhysicsSystem3::startFrame(Registry &registry)
{
    auto pool = registry.getOrCreatePool<sge::CRigidBody3>();
    auto &components = pool->getDenseComponents();
    
    for (auto &r3 : components)
    {
        // Clear entity accumulator
        r3.clearAccumulator();
    }
}

void sge::PhysicsSystem3::integrate(Registry &registry, sm::real dt)
{
    auto view = registry.viewAll<sge::CRigidBody3, sge::CTransform3>();

    // Update all entity positions with their velocities and accelerations
    for (Entity e : view)
    {
        sge::CRigidBody3 &r3 = view.get<sge::CRigidBody3>(e);
        sge::CTransform3 &t3 = view.get<sge::CTransform3>(e);

        // Infinite mass object. Continue to next entity.
        if (r3.getMass() <= 0.0f) continue;

        assert(dt > 0.0f);

        // Calculate acceleration (dependending on acting forces) and integrate velocity
        r3.velocity.addScaledVector(r3.accumulatedForce * r3.getInverseMass(), dt);
        
        // Numerical rest check
        if (r3.velocity.sqrMagnitude() < 0.00001)
        {
            r3.velocity = {0, 0, 0};
        }
        
        // Integrate position
        t3.position.addScaledVector(r3.velocity, dt);
    }
}