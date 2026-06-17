#ifndef SGE_CFAKE_STIFF_SPRING3_H
#define SGE_CFAKE_STIFF_SPRING3_H
#include <SM/Vec3.h>

namespace sge {
class CFakeStiffSpring3 {
public:
    sm::Vec3 anchorPosition;
    sm::real springConstant;
    sm::real damping;

    /**
     * Parameterized Constructor.
     * @param position the anchor position in space
     * @param springConstant the strength of the spring
     * @param damping the damping coefficient constant to apply to the force
     */
    CFakeStiffSpring3(const sm::Vec3 &position, sm::real springConstant, sm::real damping);
};
} // namespace sge

#endif // SGE_CFAKE_STIFF_SPRING3_H