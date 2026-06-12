#ifndef SGE_CABLE3_H
#define SGE_CABLE3_H
#include <SGE/systems/physics/Contact3.h>
#include <SGE/systems/physics/Link3.h>

namespace sge {
/**
 * Generates contacts for Cable-like constraints.
 * Contacts are written for entities linked together that are extended past its max length. 
 */
class Cable3 : public Link3 {
public:
    sm::real maxLength;
    sm::real restitution;

    /**
     * Fills a Contact3 pointer with the necessary data to fill for a cable like contact.
     * @param contact the Contact3* which points to 1). an array of Contact3 objects or 2). the address of a single Contact3 object
     * @param limit 1). if passing in an array, limit represents the number of Contact3 object's to loop through and write to
     *              2). if a single pointer to a single Contact3 object is passed, limit should equal 1
     * @return 0 if no cable was overextended, 1 --> limit for the number of contacts written
     */
    unsigned fillContact(Contact3 *contact, unsigned limit) const override;
};
}

#endif // SGE_CABLE3_H