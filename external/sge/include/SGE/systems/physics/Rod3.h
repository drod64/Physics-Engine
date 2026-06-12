#ifndef SGE_ROD3_H
#define SGE_ROD3_H
#include <SGE/systems/physics/Contact3.h>
#include <SGE/systems/physics/Link3.h>

namespace sge {
/**
 * Generates contacts for Rod-like constraints.
 * Contacts are written for entities linked together that are extended or compressed (relative to its length).
 */
class Rod3 : public Link3 {
public:
    sm::real length;

    /**
     * Fills a Contact3 pointer with the necessary data to fill for a rod like contact.
     * @param contact the Contact3* which points to 1). an array of Contact3 objects or 2). the address of a single Contact3 object
     * @param limit 1). if passing in an array, limit represents the number of Contact3 object's to loop through and write to
     *              2). if a single pointer to a single Contact3 object is passed, limit should equal 1
     * @return 0 if no cable was extended/compressed, 1 --> limit for the number of contacts written
     */
    unsigned fillContact(Contact3 *contact, unsigned limit) const override;
};
}

#endif // SGE_ROD3_H