#ifndef SGE_ANCHOR_SPRING3_H
#define SGE_ANCHOR_SPRING3_H
#include <SM/Precision.h>
#include <SM/Vec3.h>
#include <SGE/entities/Entity.h>
#include <SGE/systems/physics/ForceGenerator3.h>

namespace sge {
class AnchorSpring3 : ForceGenerator3 {
private:
    sm::Vec3 m_anchorPoint;
    sm::real m_springConstant;
    sm::real m_restLength;

public:
    /**
     * Parameterized Constructor.
     * @param position the anchored end point of the spring
     * @param springConstant the strength of the spring
     * @param restLength the length at which the spring is at rest
     */
    AnchorSpring3(const sm::Vec3 &position, sm::real springConstant, sm::real restLength);

    /**
     * Applies a spring force on the passed in entity.
     * @param e the entity to apply the spring force on
     * @param dt the delta time between frames
     */
    void updateForce(Entity *e, sm::real dt) override;
};
} // namespace sge

#endif // SGE_ANCHOR_SPRING3_H