#include <SGE/systems/physics/Contact3.h>

void sge::Contact3::resolve(sm::real dt)
{
    this->resolveVelocity(dt);
    this->resolveInterpenetration(dt);
}

sm::real sge::Contact3::getSeparatingVelocity() const
{
    sm::Vec3 relativeVel = this->entities[0]->getComponent<sge::CTransform3>().position;

    if (entities[1]) relativeVel -= entities[1]->getComponent<sge::CTransform3>().position;

    return relativeVel.dotProduct(contactNormal);
}

void sge::Contact3::resolveVelocity(sm::real dt)
{
    // Check if first entity exists.
    if (!this->entities[0]) return;

    // Get separating velocity.
    sm::real separatingVel = this->getSeparatingVelocity();

    // A positive separating velocity means entities are moving away from each other.
    if (separatingVel > 0.f)
    {
        return;
    }

    // Get delta velocity using refactored formula -(1 + e) * v.
    sm::real deltaVel = -(1.f + this->restitution) * separatingVel;
    
    // Acquire necessary components from components.
    auto &r3_0 = entities[0]->getComponent<sge::CRigidBody3>();
    sge::CRigidBody3 *r3_1 = (this->entities[1]) ? &this->entities[1]->getComponent<sge::CRigidBody3>() : nullptr;

    // Calculate total inverse mass.
    sm::real totalInverseMass = r3_0.getInverseMass();

    // Add second entity's mass (if it exists).
    if (r3_1)
    {
        totalInverseMass += r3_1->getInverseMass();
    }

    // Both entities are static / infinite mass.
    if (totalInverseMass <= 0.f) return;

    // Calculate impulse.
    sm::real impulse = deltaVel / totalInverseMass;
    sm::Vec3 impulsePerMass = contactNormal * impulse;
    
    // Apply impulses to entity velocities.
    r3_0.velocity += impulsePerMass * r3_0.getInverseMass();

    // Apply impulse in opposite direction for other entity (if it exists).
    if (r3_1)
    {
        r3_1->velocity += impulsePerMass * -r3_1->getInverseMass();
    }
}

void sge::Contact3::resolveInterpenetration(sm::real dt)
{
    // Early exit if entities are not interpenetrating.
    if (this->penetration <= 0) return;

    // Check if first entity exists.
    if (!this->entities[0]) return;

    // Acquire necessary components from first entity.
    auto &r3_0 = this->entities[0]->getComponent<sge::CRigidBody3>();
    auto &t3_0 = this->entities[0]->getComponent<sge::CTransform3>();

    // Acquire necessary components from second entity (if it exists).
    sge::CRigidBody3 *r3_1 = (this->entities[1]) ? &this->entities[1]->getComponent<sge::CRigidBody3>() : nullptr;
    sge::CTransform3 *t3_1 = (this->entities[1]) ? &this->entities[1]->getComponent<sge::CTransform3>() : nullptr;

    // Get total inverse mass.
    sm::real totalInverseMass = r3_0.getInverseMass();

    // Add second entity's inverse mass (if it exists).
    if (r3_1)
    {
        totalInverseMass += r3_1->getInverseMass();
    }

    // If total inverse mass is infinite, return.
    if (totalInverseMass <= 0.f) return;

    // Calculate movement vector.
    sm::Vec3 movePerInverseMass = this->contactNormal * (-this->penetration / totalInverseMass);

    // Apply movement to first entity so it is no longer interpenetrating.
    t3_0.position += movePerInverseMass * r3_0.getInverseMass();

    // If second entity exists, apply same logic.
    if (t3_1)
    {
        t3_1->position += movePerInverseMass * r3_1->getInverseMass();
    }
}