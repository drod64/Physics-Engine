#include <SGE/physics/forceComponents/CRigidBody3.h>

sge::CRigidBody3::CRigidBody3(sm::real mass, bool isStatic)
{
    setMass(mass);
    this->is_static = isStatic;
}

void sge::CRigidBody3::setMass(sm::real mass)
{
    this->inverseMass = static_cast<sm::real>(1) / mass;
}

sm::real sge::CRigidBody3::getMass() const
{
    return static_cast<sm::real>(1) / this->inverseMass;
}

void sge::CRigidBody3::addForce(const sm::Vec3 &force)
{
    this->accumulatedForce += force;
}

void sge::CRigidBody3::clearAccumulator()
{
    this->accumulatedForce = sm::Vec3(0, 0, 0);
}