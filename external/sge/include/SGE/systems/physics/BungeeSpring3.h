#ifndef SGE_BUNGEE_SPRING3_H
#define SGE_BUNGEE_SPRING3_H
#include <algorithm>
#include <SM/Precision.h>
#include <SM/Vec3.h>
#include <SGE/entities/Entity.h>
#include <SGE/systems/physics/ForceGenerator3.h>

namespace sge {
class BungeeSpring3 : public ForceGenerator3 {
private:
    Entity *m_other;
    sm::real m_springConstant;
    sm::real m_restLength;

public:
    /**
     * Parameterized Constructor.
     * @param other the other entity to connect to
     * @param springConstant the strength of the spring
     * @param restLength the length at which the spring is at rest
     */
    BungeeSpring3(Entity *other, sm::real springConstant, sm::real restLength);

    /**
     * Applies a spring (pull only) force on the passed in entity.
     * @param e the entity to apply the force on
     * @param dt the delta time between frames
     */
    void updateForce(Entity *e, sm::real dt) override;
};
}

#endif // SGE_BUNGEE_SPRING3_H