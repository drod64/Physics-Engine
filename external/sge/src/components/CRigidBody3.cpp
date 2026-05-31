#include <SGE/components/CRigidBody3.h>

sge::CRigidBody3::CRigidBody3()
:
CRigidBody3({0,0,0}, {0,0,0}, 0, 0.5)
{}

sge::CRigidBody3::CRigidBody3(const sm::Vec3 &vel, const sm::Vec3 &accumulatedForce, sm::real mass, sm::real damping)
{
    this->velocity = vel;
    this->accumulatedForce = accumulatedForce;
    setMass(mass);
    this->damping = damping;
}

void sge::CRigidBody3::setMass(sm::real mass)
{
    this->m_inverseMass = 1.0 / mass;
}

sm::real sge::CRigidBody3::getMass() const
{
    return 1.0 / this->m_inverseMass;
}