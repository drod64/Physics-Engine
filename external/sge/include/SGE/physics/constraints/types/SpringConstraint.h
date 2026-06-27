#ifndef SGE_SPRING_CONSTRAINT_H
#define SGE_SPRING_CONSTRAINT_H
#include <SGE/core/ecs/Entity.h>
#include <SM/Precision.h>

namespace sge {
class SpringConstraint {
public:
    sge::Entity targetEntity;
    sm::real springConstant;
    sm::real restLength;

    SpringConstraint(sge::Entity targetEntity, sm::real springConstant, sm::real restLength)
    {
        this->targetEntity = targetEntity;
        this->springConstant = springConstant;
        this->restLength = restLength;
    }

private:
    sm::real m_padding;
};
}

#endif // SGE_SPRING_CONSTRAINT_H