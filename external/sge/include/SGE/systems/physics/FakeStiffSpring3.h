#ifndef SGE_FAKE_SPRING3_H
#define SGE_FAKE_SPRING3_H
#include <algorithm>
#include <SM/Precision.h>
#include <SM/Vec3.h>
#include <SGE/entities/Entity.h>
#include <SGE/systems/physics/ForceGenerator3.h>

namespace sge {
// Force generator that imitates a stiff spring,
// where one end is attached to a fixed point in space.
class FakeStiffSpring3 : public ForceGenerator3 {
private:
    sm::Vec3 m_anchorPosition;
    sm::real m_springConstant;
    sm::real m_damping;

public:
    /**
     * Parameterized Constructor.
     * @param position the anchor position in space
     * @param springConstant the strength of the spring
     * @param damping the damping coefficient constant to apply to the force
     */
    FakeStiffSpring3(const sm::Vec3 &position, sm::real springConstant, sm::real damping);
    
    /**
     * Applies the (fake) stiff spring force to the passed in entity.
     * @param e the entity to apply the force on
     * @param dt the delta time between frames
     */
    void updateForce(Entity *e, sm::real dt) override;
};
} // namespace sge

#endif // SGE_FAKE_SPRING3_H