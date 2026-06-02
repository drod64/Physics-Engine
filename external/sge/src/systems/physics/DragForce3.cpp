#include <SGE/systems/physics/DragForce3.h>

sge::DragForce3::DragForce3(sm::real k1, sm::real k2)
{
    this->m_k1 = k1;
    this->m_k2 = k2;
}

void sge::DragForce3::updateForce(sge::Entity *e, sm::real dt)
{   
    auto &r3 = e->getComponent<sge::CRigidBody3>();

    sm::Vec3 force = r3.velocity;

    sm::real dragCoeff = force.magnitude();

    dragCoeff = this->m_k1 * dragCoeff + this->m_k2 *dragCoeff * dragCoeff;

    force.normalize();
    force *= -dragCoeff;

    r3.addForce(force);
}