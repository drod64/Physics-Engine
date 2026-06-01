#include <SPE/Particle3GravityForce.h>

spe::Particle3GravityForce::Particle3GravityForce(const sm::Vec3 &gravity)
{
    this->m_gravity = gravity;
}

void spe::Particle3GravityForce::updateForce(spe::Particle3 *particle, sm::real dt)
{
    // Ignore immovable particles
    if (!particle->hasFiniteMass()) return;
    // Update movable particles with gravitational force
    particle->addForce(this->m_gravity * particle->getMass());
}