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

    sm::real deltaVel = (-separatingVel * this->restitution) - separatingVel;

    sm::real totalInverseMass = entities[0]->getComponent<sge::CRigidBody3>().getInverseMass();

    if (entities[1]) totalInverseMass += entities[1]->getComponent<sge::CRigidBody3>().getInverseMass();

    if (totalInverseMass <= 0.f) return;

    sm::real impulse = deltaVel / totalInverseMass;

    sm::Vec3 impulsePerMass = contactNormal * impulse;
    
    // Apply impulses to entities.
    auto &r3 = entities[0]->getComponent<sge::CRigidBody3>();
    r3.velocity = r3.velocity + impulsePerMass * r3.getInverseMass();

    // Apply impulse in opposite direction
    if (entities[1])
    {
        auto &r31 = entities[1]->getComponent<sge::CRigidBody3>();

        r31.velocity = r31.velocity + impulsePerMass * -r3.getInverseMass();
    }
}