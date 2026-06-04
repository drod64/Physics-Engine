#ifndef SGE_FORCE_GENERATOR3_H
#define SGE_FORCE_GENERATOR3_H
#include <SM/Vec3.h>
#include <SGE/entities/Entity.h>

namespace sge {
/**
 * Base class for force generators.
 */
class ForceGenerator3 {
public:

    /**
     * Pure virtual function that must be implemented by sub-classes.
     * @param e the entity to apply the force on
     * @param dt the delta time between frames
     */
    virtual void updateForce(Entity *e, sm::real dt) = 0;
};
}

#endif // SGE_FORCE_GENERATOR_H