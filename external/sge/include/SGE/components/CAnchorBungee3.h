#ifndef SGE_CANCHOR_BUNGEE3_H
#define SGE_CANCHOR_BUNGEE3_H
#include <SM/Vec3.h>

namespace sge {
class CAnchorBungee3 {
public:
    sm::Vec3 anchorPosition;
    sm::real springConstant;
    sm::real restLength;

    /**
     * Parameterized Constructor.
     * @param position the position the entity will be anchored to
     * @param springConstant the strength of the spring
     * @param restLength the length at which the spring is at rest
     */
    CAnchorBungee3(const sm::Vec3 &position, sm::real springConstant, sm::real restLength);
};
}

#endif // SGE_CANCHOR_BUNGEE3_H
