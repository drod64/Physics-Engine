#include <SGE/systems/physics/Rod3.h>

unsigned sge::Rod3::fillContact(sge::Contact3 *contact, unsigned limit) const
{
    sm::real curLength = this->currentLength();

    // Check if rod is extended/compressed
    if (curLength == this->length)
        return 0;

    // If over/under extended, write Contact3.
    contact->entities[0] = this->entities[0];
    contact->entities[1] = this->entities[1];

    // Calculate contact normal.
    contact->contactNormal =   (this->entities[1]->getComponent<sge::CTransform3>().position -
                                this->entities[0]->getComponent<sge::CTransform3>().position).normalized();

    // The contact normal and interpenetration depth depends on if rod is extended or compressed.
    if (curLength > this->length)
    {
        contact->penetration = curLength - this->length; 
    }
    else
    {
        contact->contactNormal = contact->contactNormal * -1;
        contact->penetration = this->length - curLength;
    }

    // Rods always use a 0 restitution coefficient.
    contact->restitution = 0;

    return 1;
}