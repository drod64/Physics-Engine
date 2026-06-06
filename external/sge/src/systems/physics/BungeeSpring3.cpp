#include <SGE/systems/physics/BungeeSpring3.h>

sge::BungeeSpring3::BungeeSpring3(Entity *other, sm::real springConstant, sm::real restLength)
{
    this->m_other = other;
    this->m_springConstant = springConstant;
    this->m_restLength = restLength;
}

void sge::BungeeSpring3::updateForce(sge::Entity *e, sm::real dt)
{
    auto &t3 = e->getComponent<sge::CTransform3>();
    auto &r3 = e->getComponent<sge::CRigidBody3>();

    sm::Vec3 force = t3.position - this->m_other->getComponent<sge::CTransform3>().position;

    sm::real magnitude = force.magnitude();
    // Don't update spring force
    if (magnitude <= this->m_restLength) return;

    magnitude = this->m_springConstant * (this->m_restLength - magnitude);
    force.normalize();
    force *= -magnitude;

    r3.addForce(force);
}