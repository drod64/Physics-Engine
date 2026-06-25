#ifndef SGE_CSPRING3_H
#define SGE_CSPRING3_H
#include <SGE/core/ecs/Entity.h>
#include <SM/Precision.h>
#include <SGE/physics/linkComponents/CLink.h>

namespace sge {
class CSpring3 : public CLink<CSpring3> {
public:
    Entity targetEntity;
    sm::real springConstant;
    sm::real restLength;

    /**
     * Parameterized constructor.
     * @param targetEntity the entity to connect to
     * @param springConstant the strength of the spring
     * @param restLength the length at which the spring is at rest
     */
    CSpring3(Entity targetEntity, sm::real springConstant, sm::real restLength);
};
} // namespace sge

#endif // SGE_CSPRING3_H