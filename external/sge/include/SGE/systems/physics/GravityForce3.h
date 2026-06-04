#ifndef SGE_GRAVITY_FORCE3_H
#define SGE_GRAVITY_FORCE3_H
#include <SM/Vec3.h>
#include <SGE/entities/Entity.h>
#include <SGE/systems/physics/ForceGenerator3.h>

namespace sge {
/**
 * A force that applies gravity to an entity.
 * Sub-class that inherits from sge::ForceGenerator3
 */
class GravityForce3 :public ForceGenerator3 {
private:
    sm::Vec3 m_gravity;

public:
    /**
     * Explicit constructor that accepts the gravity force that should be applied.
     * @param gravity the desired gravity force
     */
    GravityForce3(const sm::Vec3 &gravity);

    /**
     * Applies the gravity force on the passed in entity.
     * @param e the entity to apply gravity on
     * @param dt the delta time between frames
     */
    void updateForce(Entity *e, sm::real dt) override;
};
}

#endif // SGE_GRAVITY_FORCE3_H