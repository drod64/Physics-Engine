#ifndef SGE_DRAG_FORCE3_H
#define SGE_DRAG_FORCE3_H
#include <SGE/entities/Entity.h>
#include <SGE/systems/physics/ForceGenerator3.h>

namespace sge {
/**
 * A force that applies drag to an entity.
 * Sub-class that inherits from sge::ForceGenerator3.
 */
class DragForce3 : public ForceGenerator3 {
private:
    sm::real m_k1;
    sm::real m_k2;

public:
    /**
     * Explicit constructor that takes two constants.
     * @param k1 the linear constant (dominates at low speeds)
     * @param k2 the quadratic constant (dominates at high speeds)
     */
    DragForce3(sm::real k1, sm::real k2);

    /**
     * Applies a drag force on the passed in entity.
     * @param e the entity to apply the force on
     * @param dt the delta time between frames
     */
    void updateForce(Entity *e, sm::real dt) override;
};
}

#endif // SGE_DRAG_FORCE3_H