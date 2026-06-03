#include <SPE/Particle3SpringForce.h>

spe::Particle3SpringForce::Particle3SpringForce(spe::Particle3 *other, sm::real springConstant, sm::real restLength)
{
    this->m_other = other;
    this->m_springConstant = springConstant;
    this->m_restLength = restLength;
}

void spe::Particle3SpringForce::updateForce(spe::Particle3 *particle, sm::real dt)
{
    sm::Vec3 force = particle->position;

    force -= this->m_other->position;

    sm::real magnitude = force.magnitude();
    magnitude = real_abs(magnitude - this->m_restLength);
    magnitude *= this->m_springConstant;

    force.normalize();
    force *= -magnitude;

    particle->addForce(force);
}