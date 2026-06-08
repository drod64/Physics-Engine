#ifndef SGE_ANCHOR_BUNGEE3_H
#define SGE_ANCHOR_BUNGEE3_H
#include <algorithm>
#include <SM/Precision.h>
#include <SM/Vec3.h>
#include <SGE/entities/Entity.h>
#include <SGE/systems/physics/ForceGenerator3.h>

namespace sge {
class AnchorBungee3 : public ForceGenerator3 {
private:
    sm::Vec3 m_anchorPosition;
    sm::real m_springConstant;
    sm::real m_restLength;

public:
    /**
     * Parameterized Constructor.
     * @param position the position the entity will be anchored to
     * @param springConstant the strength of the spring
     * @param restLength the length at which the spring is at rest
     */
    AnchorBungee3(const sm::Vec3 &position, sm::real springConstant, sm::real restLength);

    /**
     * Applies a spring (pull only) force on the passed in entity.
     * @param e the entity to apply the force on
     * @param dt the delta time between frames
     */
    void updateForce(Entity *e, sm::real dt) override;
};
}

#endif // SGE_ANCHOR_BUNGEE3_H