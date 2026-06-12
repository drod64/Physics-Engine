#ifndef SGE_LINK3_H
#define SGE_LINK3_H
#include <SGE/entities/Entity.h>
#include <SGE/systems/physics/Contact3.h>

namespace sge {
/**
 * Base class for linking two entities together in a 3D environment.
 * Can be used for Cables/Rods/Springs with limits.
 */
class Link3 {
public:
    Entity *entities[2];

protected:
    /**
     * Retrieves the magnitude between the linked entities.
     * @return the magntiude between the linked entities
     */
    sm::real currentLength() const;

public:
    /**
     * Fills a Contact3 pointer with the necessary data to fill with.
     * @param contact the Contact3* which points to 1). an array of Contact3 objects or 2). the address of a single Contact3 object
     * @param limit 1). if passing in an array, limit represents the number of Contact3 object's to loop through and write to
     *              2). if a single pointer to a single Contact3 object is passed, limit should equal 1
     * @return 0 if no cable was overextended, 1 --> limit for the number of contacts written
     */
    virtual unsigned fillContact(Contact3 *contact, unsigned limit) const = 0;
};
}

#endif // SGE_LINK3_H