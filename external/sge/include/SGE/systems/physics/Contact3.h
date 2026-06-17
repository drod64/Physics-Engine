#ifndef SGE_CONTACT3_H
#define SGE_CONTACT3_H
#include <SM/Vec3.h>
#include <SGE/entities/Entity.h>

namespace sge {
    
// Forward declaration of ContactResolver3 class.
class ContactResolver3;

class Contact3 {
public:
    Entity entities[2];
    sm::real restitution;
    sm::Vec3 contactNormal;
    sm::real penetration;

protected:
    /**
     * Resolves contact (in velocity and interpentration).
     * @param dt the delta time between frames
     */
    void resolve(sm::real dt);

    /**
     * Retrieves the separating velocity of this contact.
     * @return the coefficient of restitution
     */
    sm::real getSeparatingVelocity() const;

    friend class ContactResolver3;

private:
    /**
     * Calculates impulse to apply to resolve contact.
     * @param dt the delta time between frames
     */
    void resolveVelocity(sm::real dt);

    /**
     * Resolves the interpenetration between entities.
     * @param dt the delta time between frames
     */
    void resolveInterpenetration(sm::real dt);

};
} // namespace sge

#endif // SGE_CONTACT3_H