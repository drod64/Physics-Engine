#include <SGE/systems/physics/DragForce3.h>

sge::DragForce3::DragForce3(sm::real k1, sm::real k2)
{
    this->m_k1 = k1;
    this->m_k2 = k2;
}

void sge::DragForce3::updateForce(sge::Entity *e, sm::real dt)
{   
    // Get necessary component from entity.
    auto &r3 = e->getComponent<sge::CRigidBody3>();

    // Calculate speed of entity.
    sm::real speed = r3.velocity.magnitude();

    // Early exit if speed is close to 0.
    if (speed < 0.0001) return;

    // Get direction vector (velocity vector normalized).
    sm::Vec3 dragDirection = r3.velocity * ((sm::real)1 / speed);

    // Calculate drag coefficient.
    sm::real dragCoeff = this->m_k1 * speed + this->m_k2 * speed * speed;

    // Multiply direction vector by negative drag coefficient (will produce drag in the opposite way it is heading).
    r3.addForce(dragDirection * - dragCoeff);
}