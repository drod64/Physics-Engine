#ifndef SGE_BUNGEE_CONSTRAINT_H
#define SGE_BUNGEE_CONSTRAINT_H
#include <SGE/core/ecs/Entity.h>
#include <SM/Precision.h>

namespace sge {
class BungeeConstraint {
public:
    sge::Entity targetEntity;
    sm::real springConstant;
    sm::real restLength;

    BungeeConstraint(sge::Entity targetEntity, sm::real springConstant, sm::real restLength)
    {
        this->targetEntity = targetEntity;
        this->springConstant = springConstant;
        this->restLength = restLength;
    }

private:
    sm::real m_padding;
}; // class BungeeConstraint
} // namespace sge

#endif // SGE_BUNGEE_CONSTRAINT_H