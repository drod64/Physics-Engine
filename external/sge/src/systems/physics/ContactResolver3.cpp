#include <SGE/systems/physics/ContactResolver3.h>

sge::ContactResolver3::ContactResolver3(unsigned iterations)
{
    this->iterations = iterations;
    this->iterationsUsed = 0;
}

void sge::ContactResolver3::setIterations(unsigned iterations)
{
    this->iterations = iterations;
}

void sge::ContactResolver3::resolveContacts(sge::Contact3 *contact3Array, unsigned numContacts, sm::real dt)
{
    this->iterationsUsed = 0;

    while (this->iterationsUsed < iterations)
    {
        sm::real minSepVel = 0;
        unsigned chosenIndex = numContacts;

        for (unsigned i = 0; i < numContacts; ++i)
        {
            // Get separating velocity for the current contact.
            sm::real sepVel = contact3Array[i].getSeparatingVelocity();
            
            // Keep track of lowest separating velocity (value and index).
            if (sepVel < minSepVel)
            {
                minSepVel = sepVel;
                chosenIndex = i;
            }
        }

        // Resolve the contact with the lowest separating velocity (negative).
        contact3Array[chosenIndex].resolve(dt);

        ++this->iterationsUsed;
    }
}