#ifndef CBUNGEE3_H
#define CBUNGEE3_H
#include <SM/precision.h>
#include <SGE/core/ecs/Entity.h>
#include <SGE/physics/linkComponents/CLink.h>

namespace sge {
class CBungee3 : public CLink<CBungee3> {
public:
    Entity targetEntity;
    sm::real springConstant;
    sm::real restLength;

    /**
     * Parameterized Constructor.
     * @param targetEntity the other entity to connect to
     * @param springConstant the strength of the spring
     * @param restLength the length at which the spring is at rest
     */
    CBungee3(Entity targetEntity, sm::real springConstant, sm::real restLength);
};
}

#endif // CBUNGEE3_H