#include <SGE/systems/physics/AnchorSpring3.h>

sge::AnchorSpring3::AnchorSpring3(const sm::Vec3 &position, sm::real springConstant, sm::real restLength)
{
    this->m_anchorPoint = position;
    this->m_springConstant = springConstant;
    this->m_restLength = restLength;
}

void sge::AnchorSpring3::updateForce(sge::Entity *e, sm::real dt)
{
    auto &t3e = e->getComponent<sge::CTransform3>();
    auto &r3e = e->getComponent<sge::CRigidBody3>();

    sm::Vec3 force = t3e.position;

    force -= this->m_anchorPoint;

    sm::real magnitude = force.magnitude();
    magnitude = real_abs(magnitude - this->m_restLength);
    magnitude *= this->m_springConstant;

    force.normalize();
    force *= -magnitude;
    
    r3e.addForce(force);
}