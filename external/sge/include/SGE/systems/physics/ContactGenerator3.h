#ifndef SGE_CONTACT_GENERATOR3_H
#define SGE_CONTACT_GENERATOR3_H
#include <SGE/systems/physics/Contact3.h>

namespace sge {
/**
 * Base class for generating contacts.
 */
class ContactGenerator3 {
public:
    /**
     * Fills the contact structure with new generated contact(s).
     * @param contact an array or address that points to the first element in the Contact3 list
     * @param limit the maximum number of spaces allowed to be written to
     * @return the amount of contacts that have been written.
     */
    virtual unsigned addContact(Contact3 *contact, unsigned limit) const = 0;
};
}

#endif // SGE_CONTACT_GENERATOR3_H