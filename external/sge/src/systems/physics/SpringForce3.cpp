#include <SGE/systems/physics/SpringForce3.h>

sge::SpringForce3::SpringForce3(sge::Entity *other, sm::real springConstant, sm::real restLength)
{
    this->m_other = other;
    this->m_springConstant = springConstant;
    this->m_restLength = restLength;
}

void sge::SpringForce3::updateForce(sge::Entity *e, sm::real dt)
{
    // Avoid dangling pointer
    if (!this->m_other) return;

    // Retrieve necessary components from entity.
    auto &t3e = e->getComponent<sge::CTransform3>();
    auto &r3e = e->getComponent<sge::CRigidBody3>();

    // Calculate displacement between entity and other entity.
    sm::Vec3 displacement(t3e.position - this->m_other->getComponent<sge::CTransform3>().position);

    // Get length of displacement.
    sm::real length = displacement.magnitude();

    sm::Vec3 direction;
    // Prevent divide by 0
    if (length > 0.0001)
    {
        // Get normalized vector of displacement (which is the direction it is stretched/compressed).
        direction = displacement * ((sm::real)1 / length);
    }
    else
    {
        // Fallback if entity is directly on top of other entity.
        direction = {0, 1, 0};
        // Clamp length to 0.
        length = (sm::real)0;
    }
    
    // Get the stretch scalar.
    sm::real stretch = length - this->m_restLength;
    
    // Early exit if both entities are within rest length
    if (real_abs(stretch) < 0.0001) return;

    // Use Hooke's law to calculate spring force
    // F = -k * strectch
    r3e.addForce(direction * -this->m_springConstant * stretch);
}