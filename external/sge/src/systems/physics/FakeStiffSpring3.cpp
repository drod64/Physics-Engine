#include <SGE/systems/physics/FakeStiffSpring3.h>
#include <iostream>

sge::FakeStiffSpring3::FakeStiffSpring3(const sm::Vec3 &position, sm::real springConstant, sm::real damping)
{
    this->m_anchorPosition = position;
    this->m_springConstant = springConstant;
    this->m_damping = damping;
}

void sge::FakeStiffSpring3::updateForce(Entity *e, sm::real dt)
{
    auto &r3 = e->getComponent<sge::CRigidBody3>();
    auto &t3 = e->getComponent<sge::CTransform3>();

    // Early exit if entity is static.
    if (r3.isStatic()) return;

    // Get displacement vector
    sm::Vec3 displacement = t3.position - this->m_anchorPosition;

    // Calculate the constants and check whether they are in bounds.
    sm::real gamma = 0.5f * real_sqrt(4 * this->m_springConstant - this->m_damping * this->m_damping);

    if (gamma == 0.0f) return;

    sm::Vec3 c = displacement * (this->m_damping / (2.0f * gamma)) +
                r3.velocity * (1.0f / gamma);

    // Calculate the target position.
    sm::Vec3 target = displacement * real_cos(gamma * dt) +
                    c * real_sin(gamma * dt);

    target *= real_exp(-0.5f * dt * this->m_damping);

    // Calculate the resulting acceleration.
    sm::Vec3 accel = (target - displacement) * (1.0f / dt * dt) -
                    r3.velocity * dt;

    r3.addForce(accel * r3.getMass());
}