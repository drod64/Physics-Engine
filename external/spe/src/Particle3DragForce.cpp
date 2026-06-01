#include <SPE/Particle3DragForce.h>

spe::Particle3DragForce::Particle3DragForce(sm::real k1, sm::real k2)
{
    this->k1 = k1;
    this->k2 = k2;
}

void spe::Particle3DragForce::updateForce(spe::Particle3 *particle, sm::real dt)
{
    sm::Vec3 force = particle->velocity;

    sm::real dragCoeff = force.magnitude();

    dragCoeff = k1 * dragCoeff + k2 *dragCoeff * dragCoeff;

    force.normalize();
    force *= -dragCoeff;

    particle->addForce(force);
}