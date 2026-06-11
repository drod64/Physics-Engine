#ifndef SGE_CONTACT_RESOLVER3_H
#define SGE_CONTACT_RESOLVER3_H
#include <SGE/systems/physics/Contact3.h>

namespace sge {
class ContactResolver3 {
protected:
    unsigned iterations;
    unsigned iterationsUsed;

public:
    /**
     * Parameterized Constructor.
     * @param iterations the number of iterations allowed for contact resolution
     */
    ContactResolver3(unsigned iterations);

    /**
     * Sets the number of iterations allowed.
     * @param iterations the number of iterations allowed for contact resolution
     */
    void setIterations(unsigned iterations);

    /**
     * Resolves a list of contacts for both interpenetration and velocity.
     * @param contact3Array a list of Contact3 objects to be resolved
     * @param numContacts the number of contacts in the list
     * @param dt the delta time between frames 
     */
    void resolveContacts(Contact3 *contact3Array, unsigned numContacts, sm::real dt);
};
}

#endif // SGE_CONTACT_RESOLVER3_H