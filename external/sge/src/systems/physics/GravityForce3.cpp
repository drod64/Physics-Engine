#include <SGE/systems/physics/GravityForce3.h>

sge::GravityForce3::GravityForce3(const sm::Vec3 &gravity)
{
    this->m_gravity = gravity;
}

void sge::GravityForce3::updateForce(sge::Entity *e, sm::real dt)
{
    auto &r3 = e->getComponent<sge::CRigidBody3>();
    // Ignore immovable particles
    if (r3.isStatic()) return;
    // Update movable particles with gravitational force
    r3.addForce(this->m_gravity * r3.getMass());
}