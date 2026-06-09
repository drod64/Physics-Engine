#include <SGE/systems/physics/Contact3.h>

void sge::Contact3::resolve(sm::real dt)
{
    this->resolveVelocity(dt);
}

sm::real sge::Contact3::getSeparatingVelocity() const
{
    sm::Vec3 relativeVel = this->entities[0]->getComponent<sge::CTransform3>().position;

    if (entities[1]) relativeVel -= entities[1]->getComponent<sge::CTransform3>().position;

    return relativeVel.dotProduct(contactNormal);
}

void sge::Contact3::resolveVelocity(sm::real dt)
{
    sm::real separatingVel = this->getSeparatingVelocity();

    // A positive separating velocity means entities are moving away from each other.
    if (separatingVel > 0.f)
    {
        return;
    }

    // Get delta velocity using refactored formula -(1 + e) * v.
    sm::real deltaVel = -(1.f + this->restitution) * separatingVel;
    
    // Calculate total inverse mass.
    auto &r3_0 = entities[0]->getComponent<sge::CRigidBody3>();
    sm::real totalInverseMass = r3_0.getInverseMass();

    // If other entity exists, cache rigid body component...
    // ...and add inverse mass.
    sge::CRigidBody3 *r3_1 = nullptr;
    if (entities[1])
    {
        r3_1 = &entities[1]->getComponent<sge::CRigidBody3>();
        totalInverseMass += r3_1->getInverseMass();
    }

    // Both entities are static / infinite mass.
    if (totalInverseMass <= 0.f) return;

    // Calculate impulse.
    sm::real impulse = deltaVel / totalInverseMass;
    sm::Vec3 impulsePerMass = contactNormal * impulse;
    
    // Apply impulses to entity velocities.
    r3_0.velocity += impulsePerMass * r3_0.getInverseMass();

    // Apply impulse in opposite direction for other entity.
    if (entities[1])
    {
        r3_1->velocity += impulsePerMass * -r3_1->getInverseMass();
    }
}