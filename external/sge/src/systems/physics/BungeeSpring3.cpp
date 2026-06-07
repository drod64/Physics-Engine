#include <SGE/systems/physics/BungeeSpring3.h>

sge::BungeeSpring3::BungeeSpring3(Entity *other, sm::real springConstant, sm::real restLength)
{
    this->m_other = other;
    this->m_springConstant = springConstant;
    this->m_restLength = restLength;
}

void sge::BungeeSpring3::updateForce(sge::Entity *e, sm::real dt)
{
    // Retrieve necessary components from entity.
    auto &t3 = e->getComponent<sge::CTransform3>();
    auto &r3 = e->getComponent<sge::CRigidBody3>();

    // Get displacement between both entities.
    sm::Vec3 displacement = t3.position - this->m_other->getComponent<sge::CTransform3>().position;

    // Calculate squared magnitude of displacement vector.
    sm::real sqrLength = displacement.sqrMagnitude();

    // Early exit if the squared magnitude is 0.
    if (sqrLength == 0) return;

    // Cache real length of displacement vector.
    sm::real length = real_sqrt(sqrLength);
    
    // Normalize the displacement vector to get the direction it is currently stretching.
    sm::Vec3 direction = displacement * ((sm::real)1 / length);

    // Calculate the stretch
    sm::real stretch = length - this->m_restLength;
    
    // Early exit if entities are within rest length and spring is slack
    if (stretch <= 0) return;

    // Calculate the final spring force using Hooke's Law while...
    // using the direction vector to know which way it should be applied.
    // F = -k * stretch
    sm::Vec3 springForce = direction * -this->m_springConstant * stretch;

    // Add spring (pulling) force to entity.
    r3.addForce(springForce);
}