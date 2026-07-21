#include <SGE/physics/RigidBodyUtils.h>

void sge::RigidBodyUtils::addForceAtGlobalPoint(sge::CRigidBody3 &body, const sge::CTransform3 &transform,
                                                const sm::Vec3 &globalForce, const sm::Vec3 &globalPoint)
{
    // Calculate center of mass in global coords.
    sm::Vec3 globalCOM = transform.position + transform.orientation.transform(body.centerMassOffset);
    sm::Vec3 r = globalPoint - globalCOM;
    
    // Add global force.
    body.accumulatedForce += globalForce;

    // Calculate and add torque.
    body.accumulatedTorque += sm::Vec3::crossProduct(r, globalForce);
}

void sge::RigidBodyUtils::addForceAtLocalPoint(sge::CRigidBody3 &body, const sge::CTransform3 &transform,
                                                const sm::Vec3 &localForce, const sm::Vec3 &localPoint)
{
    // Calculate torque in local coords.
    sm::Vec3 r_local = localPoint - body.centerMassOffset;
    sm::Vec3 localTorque = sm::Vec3::crossProduct(r_local, localForce);

    // Add force (converted to global coords).
    body.accumulatedForce += transform.orientation.transform(localForce);

    // Add torque (converted to global coords).
    body.accumulatedTorque += transform.orientation.transform(localTorque);
}