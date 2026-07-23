#ifndef SGE_SPRING_CONSTRAINT_H
#define SGE_SPRING_CONSTRAINT_H
#include <SGE/core/ecs/Entity.h>
#include <SM/Precision.h>
#include <SM/Vec3.h>

namespace sge {
struct SpringConstraint {
public:
    Entity entityA = sge::Entity::INVALID;
    Entity entityB = sge::Entity::INVALID;
    sm::real springConstant = 10;
    sm::real restLength = 10;
    sm::Vec3 localAttachPointA;
    sm::Vec3 localAttachPointB;

}; // class SpringConstraint
} // namespace sge

#endif // SGE_SPRING_CONSTRAINT_H