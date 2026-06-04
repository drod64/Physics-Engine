#ifndef SGE_SPRING_FORCE3_H
#define SGE_SPRING_FORCE3_H
#include <SM/Precision.h>
#include <SGE/entities/Entity.h>
#include <SGE/systems/physics/ForceGenerator3.h>

namespace sge {
/**
 * A spring force that connects two entities together and
 * applies appropiate forces on them when too close/far away.
 * Sub-class that inherits from sge::ForceGenerator3.
 */
class SpringForce3 : public ForceGenerator3 {
private:
    Entity *m_other;
    sm::real m_springConstant;
    sm::real m_restLength;

public:
    /**
     * Explicit constructor that requires another entity to connect to.
     * @param other the entity to connect to
     * @param springConstant the strength of the spring
     * @param restLength the length at which the spring is at rest
     */
    SpringForce3(Entity *other, sm::real springConstant, sm::real restLength);

    /**
     * Applies the spring force on the passed in entity.
     * @param e the entity to apply the spring force on
     * @param dt the delta time between frames
     */
    void updateForce(Entity *e, sm::real dt) override;
};
}

#endif // SGE_SPRING_FORCE3_H