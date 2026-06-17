#ifndef SGE_CSPRING3_H
#define SGE_CSPRING3_H
#include <SGE/entities/Entity.h>
#include <SM/Precision.h>

namespace sge {
class CSpring3 {
public:
    Entity other;
    sm::real springConstant;
    sm::real restLength;

    /**
     * Parameterized constructor.
     * @param other the entity to connect to
     * @param springConstant the strength of the spring
     * @param restLength the length at which the spring is at rest
     */
    CSpring3(Entity other, sm::real springConstant, sm::real restLength);
};
} // namespace sge

#endif // SGE_CSPRING3_H