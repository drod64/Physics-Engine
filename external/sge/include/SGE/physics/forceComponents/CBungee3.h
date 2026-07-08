#ifndef CBUNGEE3_H
#define CBUNGEE3_H
#include <SM/precision.h>
#include <SGE/core/ecs/Entity.h>

namespace sge {
class CBungee3 {
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
    CBungee3(Entity targetEntity, sm::real springConstant, sm::real restLength)
    {
        this->targetEntity = targetEntity;
        this->springConstant = springConstant;
        this->restLength = restLength;
    }
}; // class CBungee3
} // namespace sge

#endif // CBUNGEE3_H