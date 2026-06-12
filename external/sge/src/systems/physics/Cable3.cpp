#include <SGE/systems/physics/Cable3.h>

unsigned sge::Cable3::fillContact(sge::Contact3 *contact, unsigned limit) const
{
    sm::real length = this->currentLength();

    // Check if cable is over-extending.
    if (length < this->maxLength)
        return 0;

    // If over-extended, begin building the contact.
    contact->entities[0] = this->entities[0];
    contact->entities[1] = this->entities[1];

    // Calculate the normal.
    contact->contactNormal =   (this->entities[1]->getComponent<sge::CTransform3>().position -
                                this->entities[0]->getComponent<sge::CTransform3>().position).normalized();

    contact->penetration = length - this->maxLength;
    contact->restitution = this->restitution;

    return 1;
}