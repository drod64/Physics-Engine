#ifndef SGE_CANCHOR_SPRING3_H
#define SGE_CANCHOR_SPRING3_H
#include <SM/Vec3.h>

namespace sge {
class CAnchorSpring3 {
public:
    sm::Vec3 anchorPosition;
    sm::real springConstant;
    sm::real restLength;
    sm::Vec3 localAttachPoint;

    /**
     * Parameterized Constructor.
     * @param position the anchored end point of the spring
     * @param springConstant the strength of the spring
     * @param restLength the length at which the spring is at rest
     */
    CAnchorSpring3(const sm::Vec3 &position, sm::real springConstant, sm::real restLength)
    {
        this->anchorPosition = position;
        this->springConstant = springConstant;
        this->restLength = restLength;
    }
}; // class CAnchorSpring3
} // namespace sge

#endif // SGE_CANCHOR_SPRING3_H