#include <SGE/systems/physics/IntegrationSystem3.h>

void sge::IntegrationSystem3::update(sge::Registry &registry, sge::CommandBuffer &, sm::real dt)
{
    auto view = registry.viewAll<sge::CRigidBody3, sge::CTransform3>();

    // Update all entity positions with their velocities and accelerations
    for (const Entity &e : view)
    {
        sge::CRigidBody3 &r3 = view.get<sge::CRigidBody3>(e);
        sge::CTransform3 &t3 = view.get<sge::CTransform3>(e);

        // Update previos position.
        t3.prevPosition = t3.position;
        t3.prevOrientation = t3.orientation;
        
        // Static entity. Continue to next entity.
        if (r3.is_static) continue;
        
        assert(dt > 0.0f);
        
        ////////////////////////////
        //   Linear Integration   //
        ////////////////////////////

        // Calculate acceleration (dependending on acting forces) and integrate velocity
        r3.linearVelocity.addScaledVector(r3.accumulatedForce * r3.inverseMass, dt);
        
        // Numerical rest check
        if (r3.linearVelocity.sqrMagnitude() < 0.00001)
        {
            r3.linearVelocity = {0, 0, 0};
        }
        
        // Integrate position
        t3.position.addScaledVector(r3.linearVelocity, dt);

        /////////////////////////////
        //   Angular Integration   //
        /////////////////////////////

        sm::Quaternion globalToLocal = t3.orientation.conjugated();
        sm::Vec3 localTorque = globalToLocal.transform(r3.accumulatedTorque);

        sm::Vec3 localAngularAcc(
            localTorque.x * r3.inverseInertiaTensor.x,
            localTorque.y * r3.inverseInertiaTensor.y,
            localTorque.z * r3.inverseInertiaTensor.z
        );

        sm::Vec3 worldAngularAcc = t3.orientation.transform(localAngularAcc);
        r3.angularVelocity.addScaledVector(worldAngularAcc, dt);

        if (r3.angularVelocity.sqrMagnitude() < 0.00001)
        {
            r3.angularVelocity = {0, 0, 0};
        }

        sm::Quaternion wQuat(r3.angularVelocity.x, r3.angularVelocity.y, r3.angularVelocity.z, 0);
        t3.orientation += (wQuat * t3.orientation) * (dt * 0.5);
        t3.orientation.normalize();
    }
}

sge::SystemDescriptor sge::IntegrationSystem3::getSystemDescriptor()
{
    sge::SystemDescriptor desc;

    // System phase.
    desc.phase = sge::ExecutionPhase::PhysicsIntegrate;

    // System functor.
    desc.functionPtr = &sge::IntegrationSystem3::update;

    // No system component reads.

    // No system component writes.

    // System component accumulation.
    desc.componentAccumulates.set(sge::ComponentIDCounter::get<sge::CRigidBody3>());
    desc.componentAccumulates.set(sge::ComponentIDCounter::get<sge::CTransform3>());

    // No system resource reads.

    // No system resource writes.

    // No system resource accumulation.

    // System name.
    desc.name = "IntegrationSystem3";

    return desc;
}